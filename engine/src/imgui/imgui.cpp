#include "imgui.h"
#include "imgui.h"
#include "imgui.h"

#include "../common/assert.h"
#include "../core/window.h"
#include "../core/input.h"
#include "../renderer/renderer2d.h"

#include "imguiwindow.h"
#include "imguimanager.h"

namespace prev {

	pvint ImGui::FONT_WIDTH											= 6;
	pvint ImGui::FONT_HEIGHT										= 12;

	static const pvint BUTTON_INSIDE_PADDING						= 3;
	static const pvint IMAGE_BUTTON_INSIDE_PADDING					= 6;
	static const pvint TAB_WIDTH									= ImGui::FONT_WIDTH * 2;

	static const pvint WIDGET_PADDING								= 4;
	static const pvint WINDOW_INSIDE_PADDING						= 6;
	static const pvint TITLE_BAR_PADDING							= 7;

	pvint ImGui::TITLE_BAR_HEIGHT									= ImGui::FONT_HEIGHT + TITLE_BAR_PADDING * 2;

	static const pvint SCROLL_BAR_SIZE								= 15;
	static const pvint FILLBAR_WIDTH								= 150;
	static const pvint FILLBAR_TEXT_BUFFER							= 2;
	static const pvint FILLBAR_HEIGHT								= ImGui::FONT_HEIGHT + FILLBAR_TEXT_BUFFER * 2;

	static const pvfloat ROUNDED_RADIUS								= 4.0f;

	static const Vec3 COLOR_WINDOW_BACKGROUND						= Vec3(0.15f);
	static const Vec3 COLOR_SLIDER_BTN_BORDER						= Vec3(0.0f);
	static const Vec3 COLOR_SLIDER_BG_BORDER						= Vec3(0.08f);
	static const Vec3 COLOR_SLIDER									= Vec3(0.25f);
	static const Vec3 COLOR_SLIDER_ACTIVE							= Vec3(0.55f);
	static const Vec3 COLOR_BUTTON									= Vec3(0.35f);
	static const Vec3 COLOR_BUTTON_HOVER							= Vec3(0.45f);
	static const Vec3 COLOR_BUTTON_PRESS							= Vec3(0.25f);
	static const Vec3 COLOR_BAR										= Vec3(0.15f);
	static const Vec3 COLOR_BAR_HOVER								= Vec3(0.30f);
	static const Vec3 COLOR_FILLBAR									= Vec3(0.6f, 0.6f, 0.0f);
	static const Vec3 COLOR_SEPARATOR								= Vec3(0.35f);
	static const Vec3 COLOR_WHITE									= Vec3(1.0f);
	static const Vec3 COLOR_BLACK									= Vec3(0.0f);

	static const pvint IMGUI_MOUSE_PRESS_BTN						= MOUSE_BUTTON_LEFT;
	static const pvint MOUSEWHEEL_SCROLL_DELTA						= 100;

	enum ButtonState {
		BUTTON_NONE,
		BUTTON_HOVER,
		BUTTON_PRESS,
		BUTTON_DOWN
	};

	enum SpecialWidgetID {
		ID_SCROLL_X			= (1 << 16) + 0,
		ID_SCROLL_Y			= (1 << 16) + 1,
		ID_RESIZE_BUTTON	= (1 << 16) + 2,
	};

	static inline StrongHandle<ImGuiWindow> & ImGuiWorkingWindow() { return ImGuiManager::Get()->State.WorkingWindow; }
	static inline ImGuiManager::ImGuiStateStruct & ImGuiState() { return ImGuiManager::Get()->State; }
	static inline pvbool ImGuiIsMinimized() { ASSERT(ImGuiWorkingWindow()); return (ImGuiWorkingWindow()->IsMinimized || ImGuiWorkingWindow()->IsLocked); }
	static inline pvint ImGuiTextWidth(const pvstring & str) { return ImGui::FONT_WIDTH * str.size(); }

	static inline void ImGuiPrint(const pvstring & str, Vec2i pos) {
		StrongHandle<Font> fnt = ImGuiManager::Get()->GetFont();

		Label l;
		l.Pos = ToVec2(pos);
		l.Text = str;
		l.Align = Label::Alignment::RIGHT;
		l.Scale = Vec2(10.0f);

		Renderer2D::Ref().DrawText(fnt, l);
	}

	static inline int ImguiGenWidgetId() {
		return ++(ImGuiState().WidgetCount);
	}

	static inline pvbool ImGuiIsWindowActive() {
		return ImGuiWorkingWindow() == ImGuiState().ActiveWindow;
	}

	static inline pvbool ImGuiIsWidgetActive(pvint id) {
		return ImGuiIsWindowActive() && (ImGuiManager::Get()->State.ActiveWidgetID == id);
	}

	static inline void ImGuiSetActiveWidgetID(pvint id) {
		ImGuiManager::Get()->SetActiveWidgetID(id);
	}

	static inline pvint ImGuiGetActiveWidgetID() {
		return ImGuiManager::Get()->State.ActiveWidgetID;
	}

	static inline Vec2i ImguiGetMousePos() {
		return Vec2i(Input::GetMousePosition().x, Window::Ref().GetHeight() - Input::GetMousePosition().y);
	}

	static inline pvbool ImGuiDidMouseJustGoUp() {
		return Input::IsMouseButtonReleased(IMGUI_MOUSE_PRESS_BTN);
	}

	static inline pvbool ImGuiDidMouseJustGoDown() {
		return Input::IsMouseButtonPressed(IMGUI_MOUSE_PRESS_BTN);
	}

	static inline pvbool ImGuiDidMouseDoubleClick() {
		LOG_WARN("%s not implemented", __FUNCTION__);
		return false;
	}

	static inline pvbool ImguiDidMouseWheelGoDown(pvint btn) {
		return Input::GetMouseScrollDelta().y != 0;
	}

	static inline void ImGuiColor(Vec3 col, pvfloat alpha = 1.0f) {
		Renderer2D::Ref().Color(col, alpha);
	}

	static inline void ImGuiDrawRect(Vec2i pos, Vec2i dimen) {
		Renderer2D::Ref().DrawRect(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f), ToVec2(dimen));
	}

	static inline void ImGuiDrawRectWire(Vec2i pos, Vec2i dimen) {
		Renderer2D::Ref().DrawRectWire(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f), ToVec2(dimen));
	}

	static inline void ImGuiDrawRectRounded(Vec2i pos, Vec2i dimen, pvfloat radius) {
		Renderer2D::Ref().DrawRect(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f), ToVec2(dimen));
	}

	static inline void ImGuiDrawRectRoundedWire(Vec2i pos, Vec2i dimen, pvfloat radius) {
		Renderer2D::Ref().DrawRectWire(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f), ToVec2(dimen));
	}

	static inline void ImGuiDrawRectRoundedTop(Vec2i pos, Vec2i dimen, pvfloat radius) {
		Renderer2D::Ref().DrawRect(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f), ToVec2(dimen));
	}

	static inline void ImGuiDrawLine(Vec2i start, Vec2i end) {
		Renderer2D::Ref().DrawLine(ToVec2(start), ToVec2(end));
	}

	static pvbool ImGuiMouseOver(Vec2i pos, Vec2i dimen) {
		if (ImGuiWorkingWindow()->IsLocked) return false;

		const Vec2i mousePos = ImguiGetMousePos();
		pvint mX = mousePos.x;
		pvint mY = mousePos.y;

		Vec2i regionPos = ImGuiState().MouseRegionStart + ImGuiState().ScrollOffset;
		Vec2i regionDimen = ImGuiState().MouseRegionDimen;

		return (mX >= pos.x) && (mX <= pos.x + dimen.x) && (mY <= pos.y) && (mY >= pos.y - dimen.y)
			&& (mX >= regionPos.x) && (mX <= regionPos.x + regionDimen.x) && (mY <= regionPos.y) && (mY >= regionPos.y - regionDimen.y);
	}

	static void ImGuiWindowBG(StrongHandle<ImGuiWindow> & window) {
		const float ALPHA_INACTIVE	= 0.6f;
		const float ALPHA_HOVER		= 0.8f;
		const float ALPHA_ACTIVE	= 1.0f;

		float bgAlpha = ALPHA_INACTIVE;

		// Handle minimized
		Vec2i dimen = window->Dimen;
		if (window->IsMinimized) dimen.y = ImGui::TITLE_BAR_HEIGHT;

		// Hovering over
		if (ImGuiMouseOver(window->Pos, dimen)) {
			bgAlpha = ALPHA_HOVER;

			// Current Active
			StrongHandle<ImGuiWindow> & prevActiveWindow = ImGuiState().ActiveWindow;

			// Mouse Scroll
			if (!window->AutoSize) {
				const pvint scrollDelta = MOUSEWHEEL_SCROLL_DELTA;
				if (ImguiDidMouseWheelGoDown(0)) {
					window->ScrollPos.y = Min(window->ScrollPos.y + scrollDelta, Max(1, window->DimenAutoSize.y - window->Dimen.y));
				} else if (ImguiDidMouseWheelGoDown(1)) {
					window->ScrollPos.y = Max(window->ScrollPos.y - scrollDelta, 0);
				}
			}
		}

		if (ImGuiIsWindowActive()) {
			bgAlpha = ALPHA_ACTIVE;
		}

		window->BackgroundAlpha = Lerp(window->BackgroundAlpha, bgAlpha, ImGuiIsWindowActive() ? 1.0f : 0.17f);

		if (!ImGuiIsMinimized()) {
			ImGuiColor(COLOR_WINDOW_BACKGROUND, window->BackgroundAlpha);
			ImGuiDrawRectRounded(window->Pos - Vec2i(0, ImGui::TITLE_BAR_HEIGHT), window->Dimen - Vec2i(0, ImGui::TITLE_BAR_HEIGHT), ROUNDED_RADIUS);
		}
	}

	static ButtonState ImGuiButton(const pvstring & name, pvint padding = BUTTON_INSIDE_PADDING) {
		if (ImGuiIsMinimized()) return BUTTON_NONE;

		const Vec3 COLOR_BORDER = Vec3(1.0f);
		const Vec3 COLOR_TEXT = COLOR_WHITE;

		ButtonState result = BUTTON_NONE;
		const pvint id = ImguiGenWidgetId();
		const pvint buttonHeight = ImGui::FONT_HEIGHT + padding * 2;

		const int fontWidth = ImGuiTextWidth(name);
		const Vec2i pos = ImGuiState().DrawPos;
		const Vec2i dimen = Vec2i(fontWidth + padding * 2, buttonHeight);
		Vec3 buttonColor = COLOR_BUTTON;

		// Mouse go down
		if (ImGuiMouseOver(pos, dimen)) {
			buttonColor = COLOR_BAR_HOVER;
			result = BUTTON_HOVER;

			// Mouse go down on button
			if (ImGuiDidMouseJustGoDown() && ImGuiIsWindowActive()) {
				ImGuiSetActiveWidgetID(id);
			}
			// Fully clicked on button
			else if (ImGuiDidMouseJustGoUp() && ImGuiIsWindowActive()) {
				result = BUTTON_PRESS;
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			}
		}
		// Released mouse
		else if (ImGuiDidMouseJustGoUp()) {
			if (ImGuiIsWidgetActive(id)) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			}
		}
		
		// Held down
		if (ImGuiIsWidgetActive(id)) {
			result = BUTTON_DOWN;
			buttonColor = COLOR_BUTTON_PRESS;
		}

		// Draw Button
		ImGuiColor(buttonColor);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(COLOR_BORDER);
		ImGuiDrawRectWire(pos, dimen);
		ImGuiColor(COLOR_TEXT);
		ImGuiPrint(name, pos + Vec2i(padding, -padding));

		ImGui::MoveDrawPosNextLine(dimen);

		return result;
	}

	static void ImGuiTitleBar(StrongHandle<ImGuiWindow> & window, const pvstring & name, pvint id) {
		const pvint MINIMIZED_BUTTON_PADDING = WIDGET_PADDING;
		const Vec3 COLOR_WINDOW_TITLE = Vec3(0.1f, 0.7f, 0.4f);
		const pvint BUTTON_HEIGHT = ImGui::FONT_HEIGHT + BUTTON_INSIDE_PADDING * 2;

		pvbool isWindowActive = ImGuiIsWindowActive();
		const Vec2i mousePos = ImguiGetMousePos();

		// Draw title bar background
		Vec2i titleBarDimen = Vec2i(window->Dimen.x, ImGui::TITLE_BAR_HEIGHT);
		pvfloat titleBarAlpha = 1.0f;
		Vec3 titleBarColor = COLOR_WINDOW_TITLE * window->BackgroundAlpha;
		ImGuiColor(titleBarColor, titleBarAlpha);

		// Draw rect
		if (window->IsMinimized) {
			ImGuiDrawRectRounded(window->Pos, titleBarDimen, ROUNDED_RADIUS);
		} else {
			ImGuiDrawRectRoundedTop(window->Pos, titleBarDimen, ROUNDED_RADIUS);

			// Line under title bar
			ImGuiColor(COLOR_WINDOW_BACKGROUND);
			ImGuiDrawLine(window->Pos + Vec2i(0, -ImGui::TITLE_BAR_HEIGHT), window->Pos + Vec2i(window->Dimen.x, -ImGui::TITLE_BAR_HEIGHT));
		}

		// Draw font
		pvint fontWidth = ImGuiTextWidth(name);
		pvfloat fontAlpha = isWindowActive ? 1.0f : 0.6f;
		ImGuiColor(COLOR_WHITE, fontAlpha);
		ImGuiPrint(name, window->Pos + Vec2i(TITLE_BAR_PADDING, -TITLE_BAR_PADDING));

		// Find position for button
		pvint buttonWidth = ImGui::FONT_WIDTH + BUTTON_INSIDE_PADDING * 2;
		ImGuiState().DrawPos = window->Pos + Vec2i(window->Dimen.x - buttonWidth - WIDGET_PADDING / 2 - 2, -(ImGui::TITLE_BAR_HEIGHT - BUTTON_HEIGHT) / 2);

		// Calculate how big min title bar is
		window->TitleBarMinWidth = fontWidth + TITLE_BAR_PADDING + buttonWidth + MINIMIZED_BUTTON_PADDING;

		// Handle minimized (need to unminimize to draw the button)
		pvbool minimized = window->IsMinimized;
		window->IsMinimized = false;
		ButtonState minBtn = ImGuiButton(minimized ? "+" : "-");
		minimized = minimized ^ (minBtn == BUTTON_PRESS);
		window->IsMinimized = minimized;

		// Handle title bar drag
		if (minBtn == BUTTON_NONE) {
			if (ImGuiMouseOver(window->Pos, titleBarDimen)) {
				if (ImGuiDidMouseJustGoDown() && ImGuiIsWindowActive()) {
					// double click
					if (ImGuiDidMouseDoubleClick()) {
						window->IsMinimized = !window->IsMinimized;
					}

					ImGuiSetActiveWidgetID(id);
					ImGuiState().Data.IVec[0] = mousePos.x - window->Pos.x;
					ImGuiState().Data.IVec[1] = mousePos.y - window->Pos.y;
				}
			}
		}

	}

	static pvbool ImGuiHorizontalScrollBar(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return false;
		const pvint id = ID_SCROLL_X;

		// Calculate how much bar is shown
		int totalWidthAllWidgets = window->DimenAutoSize.x;
		if (totalWidthAllWidgets <= 0) totalWidthAllWidgets = 1;
		const pvint barWidth = window->Dimen.x - 1;
		const pvfloat percentShown = static_cast<pvfloat>(barWidth) / static_cast<pvfloat>(totalWidthAllWidgets);

		// Don't show if not needed
		if (percentShown >= 1.0f) {
			window->ScrollPos.x = 0;
			return false;
		}

		// Bar
		Vec3 barColor = COLOR_BAR;
		Vec2i barPos = Vec2i(window->Pos.x + 1, window->Pos.y - window->Dimen.y + SCROLL_BAR_SIZE);
		Vec2i barDimen = Vec2i(barWidth - SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);

		// Slider
		Vec3 sliderColor = COLOR_SLIDER;
		Vec2i sliderDimen = Vec2i(static_cast<pvint>(barDimen.x * percentShown), SCROLL_BAR_SIZE);
		pvint sliderPosMinX = barPos.x;
		pvint sliderPosMaxX = barPos.x + barDimen.x - sliderDimen.x;
		pvfloat scrollRatioX = Saturate(static_cast<pvfloat>(window->ScrollPos.x) / (Max(window->DimenAutoSize.x, window->DimenAutoSizePrev.x) - window->Dimen.x));
		Vec2i sliderPos = Vec2i(static_cast<pvint>(Lerp(static_cast<pvfloat>(sliderPosMinX), static_cast<pvfloat>(sliderPosMaxX), scrollRatioX), barPos.y));

		const pvint mouseX = ImguiGetMousePos().x;

		// Mosue Over
		if (ImGuiMouseOver(barPos, barDimen)) {
			barColor = COLOR_BAR_HOVER;

			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetID(id);
				if (ImGuiMouseOver(sliderPos, sliderDimen)) { // Over Slider
					ImGuiState().Data.I = mouseX - sliderPos.x;
				} else { // Not over slider, chose half the position
					ImGuiState().Data.I = sliderDimen.x >> 1;
				}
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// Released mouse
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			}
			// Bar position based on mouse pos
			else {
				pvint xMin = barPos.x + ImGuiState().Data.I;
				pvint xMax = barPos.x + barDimen.x - sliderDimen.x + ImGuiState().Data.I;

				scrollRatioX = Saturate(static_cast<pvfloat>(mouseX - xMin) / static_cast<pvfloat>(xMax - xMin));
				window->ScrollPos.x = static_cast<pvint>(scrollRatioX * (Max(window->DimenAutoSize.x, window->DimenAutoSizePrev.x) - window->Dimen.x));
			}
		}

		if (ImGuiIsWidgetActive(id)) sliderColor = COLOR_SLIDER_ACTIVE;

		// Draw background
		ImGuiColor(barColor);
		ImGuiDrawRect(barPos, barDimen);
		ImGuiColor(COLOR_SLIDER_BG_BORDER);
		ImGuiDrawRectWire(barPos, barDimen);

		// Draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRectRounded(sliderPos, sliderDimen, ROUNDED_RADIUS);
		ImGuiColor(COLOR_SLIDER_BTN_BORDER);
		ImGuiDrawRectRoundedWire(sliderPos, sliderDimen, ROUNDED_RADIUS);

		return true;
	}

	static pvbool ImGuiVerticalScrollBar(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return false;
		const pvint id = ID_SCROLL_Y;

		// Calculate how much bar is shown
		int totalHeightAllWidgets = window->DimenAutoSize.y - ImGui::TITLE_BAR_HEIGHT;
		if (totalHeightAllWidgets <= 0) totalHeightAllWidgets = 1;
		const pvint barHeight = window->Dimen.y - ImGui::TITLE_BAR_HEIGHT;
		const pvfloat percentShown = static_cast<pvfloat>(barHeight) / static_cast<pvfloat>(totalHeightAllWidgets);

		// Don't show if not needed
		if (percentShown >= 1.0f) {
			window->ScrollPos.y = 0;
			return false;
		}

		// Bar
		Vec3 barColor = COLOR_BAR;
		Vec2i barPos = Vec2i(window->Pos.x + window->Dimen.x - SCROLL_BAR_SIZE, window->Pos.y - ImGui::TITLE_BAR_HEIGHT);
		Vec2i barDimen = Vec2i(SCROLL_BAR_SIZE, barHeight - SCROLL_BAR_SIZE);

		// Slider
		Vec3 sliderColor = COLOR_SLIDER;
		Vec2i sliderDimen = Vec2i(SCROLL_BAR_SIZE, static_cast<pvint>(barDimen.x * percentShown));
		pvint sliderPosMinY = barPos.y;
		pvint sliderPosMaxY = barPos.y - barDimen.y + sliderDimen.y;
		pvfloat scrollRatioY = Saturate(static_cast<pvfloat>(window->ScrollPos.y) / static_cast<pvfloat>(Max(window->DimenAutoSize.y, window->DimenAutoSizePrev.y) - window->Dimen.y));
		Vec2i sliderPos = Vec2i(barPos.x, static_cast<pvint>(Lerp(static_cast<pvfloat>(sliderPosMinY), static_cast<pvfloat>(sliderPosMaxY), scrollRatioY)));

		const pvint mouseY = ImguiGetMousePos().y;

		// Mosue Over
		if (ImGuiMouseOver(barPos, barDimen)) {
			barColor = COLOR_BAR_HOVER;

			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetID(id);
				if (ImGuiMouseOver(sliderPos, sliderDimen)) { // Over Slider
					ImGuiState().Data.I = sliderPos.y - mouseY;
				} else { // Not over slider, chose half the position
					ImGuiState().Data.I = sliderDimen.y >> 1;
				}
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// Released mouse
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			}
			// Bar position based on mouse pos
			else {
				pvint yMin = barPos.y - ImGuiState().Data.I;
				pvint yMax = barPos.y - barDimen.y - sliderDimen.y + ImGuiState().Data.I;

				scrollRatioY = Saturate(static_cast<pvfloat>(mouseY - yMin) / static_cast<pvfloat>(yMax - yMin));
				window->ScrollPos.y = static_cast<pvint>(scrollRatioY * (Max(window->DimenAutoSize.y, window->DimenAutoSizePrev.y) - window->Dimen.y));
			}
		}

		if (ImGuiIsWidgetActive(id)) sliderColor = COLOR_SLIDER_ACTIVE;

		// Draw background
		ImGuiColor(barColor);
		ImGuiDrawRect(barPos, barDimen);
		ImGuiColor(COLOR_SLIDER_BG_BORDER);
		ImGuiDrawRectWire(barPos, barDimen);

		// Draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRectRounded(sliderPos, sliderDimen, ROUNDED_RADIUS);
		ImGuiColor(COLOR_SLIDER_BTN_BORDER);
		ImGuiDrawRectRoundedWire(sliderPos, sliderDimen, ROUNDED_RADIUS);

		return true;
	}

	static inline Vec2i ImGuiGetScrollBarDimenOffset(StrongHandle<ImGuiWindow> & window) {
		pvint dimenX = (window->Dimen.y < window->DimenAutoSize.y && !window->AutoSize) ? SCROLL_BAR_SIZE : 0;
		pvint dimenY = (window->Dimen.x < window->DimenAutoSize.x && !window->AutoSize) ? SCROLL_BAR_SIZE + 1 : 0;
		return Vec2i(dimenX, dimenY);
	}

	static void ImGuiResizeButton(StrongHandle<ImGuiWindow> & window) {
		if (ImGuiIsMinimized()) return;
		const pvint id = ID_RESIZE_BUTTON;

		const Vec2i pos = window->Pos + Vec2i(window->Dimen.x, -window->Dimen.y) + Vec2i(-SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);
		const Vec2i dimen = Vec2i(SCROLL_BAR_SIZE);
		const Vec2i mousePos = ImguiGetMousePos();

		Vec3 color = COLOR_SLIDER;

		// Mouse over
		if (ImGuiMouseOver(pos, dimen)) {
			color = COLOR_BUTTON_HOVER;
			// Clicked on button
			if (ImGuiDidMouseJustGoDown()) {
				ImGuiSetActiveWidgetID(id);

				// Cache mouse position and offset
				ImGuiState().Data.IVec[0] = mousePos.x;
				ImGuiState().Data.IVec[1] = mousePos.y;
				ImGuiState().Data.IVec[2] = mousePos.x - pos.x;
				ImGuiState().Data.IVec[3] = mousePos.y - pos.y;
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			color = COLOR_BUTTON_PRESS;
			// Released
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			} else {
				//Resize window
				window->Dimen.x = Max(window->TitleBarMinWidth + SCROLL_BAR_SIZE, mousePos.x - window->Pos.x - ImGuiState().Data.IVec[2] + SCROLL_BAR_SIZE);
				window->Dimen.y = Max(ImGui::TITLE_BAR_HEIGHT + SCROLL_BAR_SIZE, window->Pos.y - mousePos.y + ImGuiState().Data.IVec[3] + SCROLL_BAR_SIZE);
			}
		}

		// Background
		ImGuiColor(color);
		ImGuiDrawRect(pos, dimen);
		ImGuiColor(COLOR_BLACK);
		ImGuiDrawRectWire(pos, dimen);

		// Line
		const pvint LINE_PADDING = 2;
		Vec2i linePosStart = pos + Vec2i(LINE_PADDING, -SCROLL_BAR_SIZE + LINE_PADDING);
		Vec2i linePosEnd = pos + Vec2i(SCROLL_BAR_SIZE - LINE_PADDING, -LINE_PADDING);
		ImGuiColor(COLOR_WHITE, ImGuiIsWidgetActive(id) ? 0.85f : 0.5f);
		ImGuiDrawLine(linePosStart, linePosEnd);
	}

	static void ImGuiRefreshScrollOffset(pvbool begin = false) {
		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();

		// Figure our translation offset
		pvint translateX = -window->ScrollPos.x;
		pvint translateY =  window->ScrollPos.y;
		ImGuiState().ScrollOffset = -Vec2i(translateX, translateY);

		//LOG_WARN("Set translation");
	}

	void ImGui::Begin(pvstring name, Vec2i pos, Vec2i dimen) {
		ASSERTM(name.size() > 0, "Invalid Window name");
		if (ImGuiWorkingWindow() != nullptr) {
			LOG_ERROR("ImGui::Begin('%s') Error - Forgot to call ImGui::End() on window '%s'",
				name.c_str(), ImGuiState().WorkingWindow->Name.c_str());
			ASSERTM(false, "Forgot to call ImGui::End");
		}

		const pvint id = ImguiGenWidgetId();
		ASSERT(id == ImGuiManager::ImGuiStateStruct::WIDGET_WINDOW);
		const Vec2i mousePos = ImguiGetMousePos();
		const Vec2i windowSize = Vec2i(Window::Ref().GetWidth(), Window::Ref().GetHeight());

		// Set Render
		//
		// ----------

		// Get Window
		ImGuiManager * mgr = ImGuiManager::Get();
		StrongHandle<ImGuiWindow> window = mgr->GetWindow(name);
		window->FramesSinceUpdate = 0;
		ImGuiState().WorkingWindow = window;
		
		// Remove tabbing
		ImGuiState().NumTabs = 0;

		// On first creation of window
		if (window->IsNewWindow) {

			// Dimension
			window->AutoSize = (dimen == Vec2i(ImGui::AUTOSIZE));

			// AutoSize
			if (window->AutoSize) window->Dimen = Vec2i(0);

			// If not auto-sized
			else if (window->Dimen == Vec2i(ImGui::AUTOSIZE) || window->Dimen == Vec2i(0)) window->Dimen = dimen;

			// This condition is for new windows with position already set (Like from a file)
			if (window->Pos == Vec2i(-1)) {
				window->Pos = pos;
			}

			// Calculate autopos
			if (window->Pos == Vec2i(ImGui::AUTOPOS)) {
				const pvint windowMargin = 150;
				static Vec2i autoPos = Vec2i(windowMargin, windowSize.y - 10);

				window->Pos = autoPos;

				// Move auto-pos cursor
				autoPos += Vec2i(windowMargin, 0);

				// overflow to next level
				if (autoPos.x > windowSize.x - windowMargin * 2) {
					autoPos = Vec2i(windowMargin, autoPos.y - windowMargin * 2);
				}
			}
		}

		// Handle dragging
		if (ImGuiIsWidgetActive(id)) {
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			} else {
				Vec2i delta = Vec2i(ImGuiState().Data.IVec[0], ImGuiState().Data.IVec[1]);
				window->Pos = mousePos - delta;
			}
		}

		// Border dimen
		Vec2i borderDimen = window->Dimen;
		if (window->IsMinimized || window->IsLocked) borderDimen.y = ImGui::TITLE_BAR_HEIGHT;

		// Border Shadow
		const int shadowOffset = 1;
		const int shadowSize = 4;
		ImGuiColor(COLOR_BLACK, 0.2f);
		ImGuiDrawRectRounded(window->Pos + Vec2i(shadowOffset, -shadowOffset), borderDimen + Vec2i(shadowSize), ROUNDED_RADIUS);

		ImGuiWindowBG(window);
		ImGuiTitleBar(window, name, id);

		// Rounded border
		ImGuiColor(COLOR_WINDOW_BACKGROUND * 0.75f);
		ImGuiDrawRectRoundedWire(window->Pos, borderDimen, ROUNDED_RADIUS);

		// Show scroll bars
		if (!window->AutoSize) {
			pvbool showHorizontalScroll = ImGuiHorizontalScrollBar(window);
			pvbool showVerticalScroll = ImGuiVerticalScrollBar(window);
			
			// Only show resize button if any scrollbars are visible
			if (showHorizontalScroll || showVerticalScroll) {
				ImGuiResizeButton(window);
			}
		}

		const Vec2i scrollBarDimenOffset = ImGuiGetScrollBarDimenOffset(ImGuiWorkingWindow());
		// Set scissors
		//
		// ------------

		ImGuiRefreshScrollOffset(true);

		// Where mouse can interact
		ImGuiState().MouseRegionStart = window->Pos - Vec2i(0, ImGui::TITLE_BAR_HEIGHT);
		ImGuiState().MouseRegionDimen = window->Dimen - Vec2i(0, ImGui::TITLE_BAR_HEIGHT) - scrollBarDimenOffset;

		// Determine size through ImGui calls
		window->DimenAutoSizePrev = window->DimenAutoSize;
		window->DimenAutoSize = Vec2i(0);
		MoveDrawPosNextLine(Vec2i(0, WINDOW_INSIDE_PADDING - WIDGET_PADDING));
	}

	void ImGui::End() {
		ASSERTM(ImGuiWorkingWindow() != nullptr, "ImGui::End() called without existing working window");

		// Editable region
		const Vec2i windowSize = Vec2i(Window::Get()->GetWidth(), Window::Get()->GetHeight());
		ImGuiState().MouseRegionStart = Vec2i(0, windowSize.y);
		ImGuiState().MouseRegionDimen = windowSize;
		ImGuiState().ScrollOffset = Vec2i(0);

		// End render state
		//
		// ----------------

		// Disable working window
		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();
		window->IsNewWindow = false;

		// Will use title bar width if that is bigger
		window->DimenAutoSize.x = Max(window->DimenAutoSize.x, window->TitleBarMinWidth);

		// Add window padding
		window->DimenAutoSize += Vec2i(WINDOW_INSIDE_PADDING, WINDOW_INSIDE_PADDING - WIDGET_PADDING);

		// If autosized
		if (window->AutoSize) window->Dimen = window->DimenAutoSize;

		// Did use click anywhere in box
		const pvbool isMouseInWindow = ImGuiMouseOver(window->Pos, window->Dimen);
		const pvbool mouseClicked = ImGuiDidMouseJustGoDown();

		// If currently active window
		if (ImGuiIsWindowActive()) {

			// If no widget is selected and use clicked on the widget, set it to drag
			if (ImGuiGetActiveWidgetID() == ImGuiManager::ImGuiStateStruct::WIDGET_NULL &&
				isMouseInWindow && mouseClicked && !window->IsMinimized) {
				Vec2i mousePos = ImguiGetMousePos();
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_WINDOW);
				ImGuiState().Data.IVec[0] = mousePos.x - window->Pos.x;
				ImGuiState().Data.IVec[1] = mousePos.y - window->Pos.y;
			}
		}

		ImGuiState().WorkingWindow = nullptr;
		ImGuiState().WidgetCount = 0;
	}

	void ImGui::MoveDrawPosBy(Vec2i dimen) {
		StrongHandle<ImGuiWindow> & window = ImGuiWorkingWindow();

		ImGuiState().DrawPosPrev = ImGuiState().DrawPos;
		ImGuiState().DrawPosPrev.x += dimen.x + WIDGET_PADDING;
		ImGuiState().DrawPos += Vec2i(dimen.x, -dimen.y);

		window->DimenAutoSize.x = Max(window->DimenAutoSize.x, ImGuiState().DrawPos.x - window->Pos.x);
		window->DimenAutoSize.y = Max(window->DimenAutoSize.y, window->Pos.y - ImGuiState().DrawPos.y);
	}

	void ImGui::MoveDrawPosNextLine(Vec2i dimen) {
		StrongHandle<ImGuiWindow> & window = ImGuiWorkingWindow();

		ImGui::MoveDrawPosBy(dimen + Vec2i(0, WIDGET_PADDING));
		ImGuiState().DrawPos.x = window->Pos.x + WINDOW_INSIDE_PADDING + TAB_WIDTH * ImGuiState().NumTabs;
	}

}