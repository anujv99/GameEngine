#include "ImGui.h"
#include "ImGui.h"
#include "ImGui.h"

#include "../common/assert.h"
#include "../core/window.h"
#include "../core/input.h"
#include "../renderer/renderer2d.h"

#include "ImGuiwindow.h"
#include "ImGuimanager.h"

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

	static Vec3 COLOR_WINDOW_BACKGROUND								= Vec3(0.2f);
	static Vec3 COLOR_WINDOW_LINE_UNDER_TTILE						= Vec3(1.0f);
	static Vec3 COLOR_WINDOW_TITLE									= Vec3(0.1f, 0.7f, 0.4f);
	static Vec3 COLOR_SLIDER_BTN_BORDER								= Vec3(0.0f);
	static Vec3 COLOR_SLIDER_BG_BORDER								= Vec3(0.08f);
	static Vec3 COLOR_SLIDER										= Vec3(0.25f);
	static Vec3 COLOR_SLIDER_ACTIVE									= Vec3(0.55f);
	static Vec3 COLOR_BUTTON										= Vec3(0.35f);
	static Vec3 COLOR_BUTTON_HOVER									= Vec3(0.45f);
	static Vec3 COLOR_BUTTON_PRESS									= Vec3(0.25f);
	static Vec3 COLOR_BAR											= Vec3(0.15f);
	static Vec3 COLOR_BAR_HOVER										= Vec3(0.30f);
	static Vec3 COLOR_FILLBAR										= Vec3(0.6f, 0.6f, 0.0f);
	static Vec3 COLOR_SEPARATOR										= Vec3(0.35f);
	static Vec3 COLOR_WHITE											= Vec3(1.0f);
	static Vec3 COLOR_BLACK											= Vec3(0.0f);

	static const pvint ImGui_MOUSE_PRESS_BTN						= MOUSE_BUTTON_LEFT;
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
		static const Vec2 PADDING = Vec2(0.0f, -5.0f);

		StrongHandle<ImGuiManager::WindowDrawCall> call = ImGuiManager::Get()->GetDrawCall(ImGuiWorkingWindow());
		call->Text.push_back(str);
		call->TextColor.push_back(ImGuiManager::Get()->m_CurrentColor);
		call->TextPos.push_back(ToVec2(pos) + PADDING);
	}

	static inline int ImGuiGenWidgetID() {
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

	static inline Vec2i ImGuiGetMousePos() {
		return Vec2i(Input::GetMousePosition().x, Window::Ref().GetHeight() - Input::GetMousePosition().y);
	}

	static inline pvbool ImGuiDidMouseJustGoUp() {
		return Input::IsMouseButtonReleased(ImGui_MOUSE_PRESS_BTN);
	}

	static inline pvbool ImGuiDidMouseJustGoDown() {
		return Input::IsMouseButtonPressed(ImGui_MOUSE_PRESS_BTN);
	}

	static inline pvbool ImGuiDidMouseDoubleClick() {
		LOG_WARN("%s not implemented", __FUNCTION__);
		return false;
	}

	static inline pvbool ImGuiDidKeyJustGoDown(pvushort key) {
		return Input::IsKeyPressed(key);
	}

	static inline pvbool ImGuiIsKeyDown(pvushort key) {
		return Input::IsKeyDown(key);
	}

	static inline std::vector<pvchar> ImGuiGetKeyPressed() {
		return Input::GetPressedCharacterBuffer();
	}

	static inline pvbool ImGuiDidMouseWheelGoDown(pvint btn) {
		return Input::GetMouseScrollDelta().y != 0;
	}

	static inline void ImGuiColor(Vec3 col, pvfloat alpha = 1.0f) {
		ImGuiManager::Ref().m_CurrentColor = Vec4(col.x, col.y, col.z, alpha);
	}

	static inline void ImGuiDrawRect(Vec2i pos, Vec2i dimen) {
		StrongHandle<ImGuiManager::WindowDrawCall> call = ImGuiManager::Get()->GetDrawCall(ImGuiWorkingWindow());
		call->QuadPos.push_back(Vec2(pos.x + dimen.x / 2.0f, pos.y - dimen.y / 2.0f));
		call->QuadDimen.push_back(ToVec2(dimen));
		call->QuadColor.push_back(ImGuiManager::Ref().m_CurrentColor);
	}

	static inline void ImGuiDrawLine(Vec2i start, Vec2i end) {
		StrongHandle<ImGuiManager::WindowDrawCall> call = ImGuiManager::Get()->GetDrawCall(ImGuiWorkingWindow());
		call->QuadPos.push_back(Vec2(start.x + (end.x - start.x) / 2.0f, start.y));
		call->QuadDimen.push_back(Vec2(end.x - start.x, 1.0f));
		call->QuadColor.push_back(ImGuiManager::Ref().m_CurrentColor);
	}

	static pvbool ImGuiMouseOver(Vec2i pos, Vec2i dimen) {
		if (ImGuiWorkingWindow()->IsLocked) return false;

		const Vec2i mousePos = ImGuiGetMousePos();
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
				if (ImGuiDidMouseWheelGoDown(0)) {
					window->ScrollPos.y = Min(window->ScrollPos.y + scrollDelta, Max(1, window->DimenAutoSize.y - window->Dimen.y));
				} else if (ImGuiDidMouseWheelGoDown(1)) {
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
			ImGuiDrawRect(window->Pos - Vec2i(0, ImGui::TITLE_BAR_HEIGHT), window->Dimen - Vec2i(0, ImGui::TITLE_BAR_HEIGHT));
		}
	}

	static ButtonState ImGuiButton(const pvstring & name, pvint padding = BUTTON_INSIDE_PADDING) {
		if (ImGuiIsMinimized()) return BUTTON_NONE;

		const Vec3 COLOR_BORDER = Vec3(1.0f);
		const Vec3 COLOR_TEXT = COLOR_WHITE;

		ButtonState result = BUTTON_NONE;
		const pvint id = ImGuiGenWidgetID();
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
		ImGuiColor(COLOR_TEXT);
		ImGuiPrint(name, pos + Vec2i(padding, -padding));

		ImGui::MoveDrawPosNextLine(dimen);

		return result;
	}

	static void ImGuiTitleBar(StrongHandle<ImGuiWindow> & window, const pvstring & name, pvint id) {
		const pvint MINIMIZED_BUTTON_PADDING = WIDGET_PADDING;
		const pvint BUTTON_HEIGHT = ImGui::FONT_HEIGHT + BUTTON_INSIDE_PADDING * 2;

		pvbool isWindowActive = ImGuiIsWindowActive();
		const Vec2i mousePos = ImGuiGetMousePos();

		// Draw title bar background
		Vec2i titleBarDimen = Vec2i(window->Dimen.x, ImGui::TITLE_BAR_HEIGHT);
		pvfloat titleBarAlpha = 1.0f;
		Vec3 titleBarColor = COLOR_WINDOW_TITLE * window->BackgroundAlpha;
		ImGuiColor(titleBarColor, titleBarAlpha);

		// Draw rect
		if (window->IsMinimized) {
			ImGuiDrawRect(window->Pos, titleBarDimen);
		} else {
			ImGuiDrawRect(window->Pos, titleBarDimen);

			// Line under title bar
			ImGuiColor(COLOR_WINDOW_LINE_UNDER_TTILE);
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

		const pvint mouseX = ImGuiGetMousePos().x;

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

		// Draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRect(sliderPos, sliderDimen);

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

		const pvint mouseY = ImGuiGetMousePos().y;

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

		// Draw slider
		ImGuiColor(sliderColor);
		ImGuiDrawRect(sliderPos, sliderDimen);

		return true;
	}

	static pvfloat ImGuiSlider(pvint id, pvfloat percent) {
		const pvint SLIDER_WIDTH = 150;
		const pvint SLIDER_HEIGHT = ImGui::FONT_HEIGHT;
		const pvint BUTTON_WIDTH = 10;

		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(SLIDER_WIDTH, SLIDER_HEIGHT);

		// Extend size if not autosized
		if (!ImGuiWorkingWindow()->AutoSize) {
			dimen.x = Max(dimen.x, ImGuiWorkingWindow()->Dimen.x - WIDGET_PADDING * 2 - WINDOW_INSIDE_PADDING * 2 - ImGui::FONT_WIDTH * 15);
		}

		Vec3 colorBar = COLOR_BAR;
		Vec3 colorButton = COLOR_SLIDER;

		//Button position
		pvint xMin = pos.x + BUTTON_WIDTH / 2;
		pvint xMax = pos.x + dimen.x - BUTTON_WIDTH / 2;

		if (ImGuiMouseOver(pos, dimen)) {
			colorBar = COLOR_BAR_HOVER;

			// clicked down
			if (ImGuiDidMouseJustGoDown() && ImGuiIsWindowActive()) {
				ImGuiSetActiveWidgetID(id);
				ImGuiState().Data.F = 0.0f;
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			// released
			if (ImGuiDidMouseJustGoUp()) {
				ImGuiSetActiveWidgetID(ImGuiManager::ImGuiStateStruct::WIDGET_NULL);
			}

			pvint mouseX = ImGuiGetMousePos().x;
			percent = Saturate(pvfloat(mouseX - xMin) / (xMax - xMin));

			colorButton = COLOR_SLIDER_ACTIVE;
		}

		// background
		ImGuiColor(colorBar);
		ImGuiDrawRect(pos, dimen);

		// draw button
		pvint xPos = (pvint)Lerp((pvfloat)xMin, (pvfloat)xMax, Saturate(percent));
		Vec2i buttonDimen = Vec2i(BUTTON_WIDTH, SLIDER_HEIGHT - 1);
		Vec2i buttonPos = Vec2i(xPos - BUTTON_WIDTH / 2, pos.y);
		ImGuiColor(colorButton);
		ImGuiDrawRect(buttonPos, buttonDimen);

		// move draw cursor
		ImGui::MoveDrawPosNextLine(dimen);

		return percent;
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
		const Vec2i mousePos = ImGuiGetMousePos();

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

		const pvint id = ImGuiGenWidgetID();
		ASSERT(id == ImGuiManager::ImGuiStateStruct::WIDGET_WINDOW);
		const Vec2i mousePos = ImGuiGetMousePos();
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
		ImGuiDrawRect(window->Pos + Vec2i(shadowOffset, -shadowOffset), borderDimen + Vec2i(shadowSize));

		ImGuiWindowBG(window);
		ImGuiTitleBar(window, name, id);

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
				Vec2i mousePos = ImGuiGetMousePos();
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

	void ImGui::SameLine() {
		if (ImGuiIsMinimized()) return;
		ImGuiState().DrawPos = ImGuiState().DrawPosPrev;
	}

	void ImGui::Print(pvstring str) {
		if (ImGuiIsMinimized() || str.size() <= 0) return;

		pvint textWidth = ImGuiTextWidth(str);
		ImGuiColor(COLOR_WHITE);
		ImGuiPrint(str, ImGuiState().DrawPos);
		MoveDrawPosNextLine(Vec2i(textWidth, ImGui::FONT_HEIGHT));
	}

	void ImGui::Seperator() {
		if (ImGuiIsMinimized()) return;

		const pvint PADDING = WIDGET_PADDING;

		StrongHandle<ImGuiWindow> window = ImGuiWorkingWindow();

		Vec2i pos = ImGuiState().DrawPos - Vec2i(0, PADDING);
		Vec2i dimen = Vec2i(window->Dimen.x, 0);

		ImGuiColor(COLOR_BLACK, 0.6f);
		ImGuiDrawLine(pos, pos + Vec2i(window->Dimen.x - WINDOW_INSIDE_PADDING * 2, 0));

		ImGuiColor(COLOR_SEPARATOR);
		ImGuiDrawLine(pos - Vec2i(0, 1), pos - Vec2i(0, 1) + Vec2i(window->Dimen.x - WINDOW_INSIDE_PADDING * 2, 0));

		MoveDrawPosNextLine(Vec2i(1, dimen.y + PADDING * 2 + 1));
	}

	// Widgets

	pvfloat ImGui::SliderFloat(pvstring name, pvfloat & val, pvfloat min, pvfloat max) {
		if (ImGuiIsMinimized()) return val;
		const pvint id = ImGuiGenWidgetID();

		Print(name);

		// Value
		pvfloat sliderVal = min + ImGuiSlider(id, (val - min) / (max - min)) * (max - min);
		val = sliderVal;

		// Print Text
		SameLine();
		Print(std::to_string(val));

		return val;
	}

	Vec3 ImGui::SliderRGB(pvstring name, Vec3 & val) {
		if (ImGuiIsMinimized()) return val;
		ImGuiGenWidgetID();

		Print(name);

		// Draw color block

		SameLine();
		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(ImGui::FONT_HEIGHT * 2, ImGui::FONT_HEIGHT);
		ImGuiColor(val);
		ImGuiDrawRect(pos, dimen);
		MoveDrawPosNextLine(dimen);

		const Vec3 min(0.0f);
		const Vec3 max(1.0f);

		SliderFloat(nullptr, val.x, min.x, max.x);
		SliderFloat(nullptr, val.y, min.y, max.y);
		SliderFloat(nullptr, val.z, min.z, max.z);

		return val;
	}

	Vec4 ImGui::SliderRGBA(pvstring name, Vec4 & val) {
		if (ImGuiIsMinimized()) return val;
		ImGuiGenWidgetID();

		Print(name);

		// Draw color block

		SameLine();
		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(ImGui::FONT_HEIGHT * 2, ImGui::FONT_HEIGHT);
		ImGuiColor(val.xyz(), val.w);
		ImGuiDrawRect(pos, dimen);
		MoveDrawPosNextLine(dimen);

		const Vec4 min(0.0f);
		const Vec4 max(1.0f);

		SliderFloat(nullptr, val.x, min.x, max.x);
		SliderFloat(nullptr, val.y, min.y, max.y);
		SliderFloat(nullptr, val.z, min.z, max.z);
		SliderFloat(nullptr, val.w, min.w, max.w);

		return val;
	}

	int ImGui::TextInput(const pvstring & name, pvstring & val, pvint width) {

		const pvfloat KEYPRESS_COOLDOWN = 0.4f;
		const pvfloat KEYHOLD_COOLDOWN = 0.04f;

		if (ImGuiIsMinimized()) return 0;
		const pvint id = ImGuiGenWidgetID();

		//Title
		ImGui::Print(name);

		Vec2i pos = ImGuiState().DrawPos;
		Vec2i dimen = Vec2i(width, FILLBAR_HEIGHT);
		Vec3 colorBar = COLOR_BAR;
		Vec3 colorBorder = COLOR_BLACK;
		pvint numCharSlots = (dimen.x - FILLBAR_TEXT_BUFFER * 2) / ImGui::FONT_WIDTH;

		//Get carret pos
		pvint & caretStart = ImGuiState().Data.IVec[0];
		pvint & caretPos = ImGuiState().Data.IVec[1];

		if (ImGuiIsWidgetActive(id)) {
			caretStart = Clamp(caretStart, 0, Max(0, (pvint)val.size() - 1));
			caretPos = Clamp(caretPos, caretStart, caretStart + Min(numCharSlots - 1, (pvint)val.size()));
		}

		pvfloat & heldKeyCooldown = ImGuiState().Data.FVec[2];

		if (ImGuiMouseOver(pos, dimen)) {
			colorBar = COLOR_BAR_HOVER;

			//Clicked Down
			if (ImGuiDidMouseJustGoDown() && !ImGuiIsWidgetActive(id) && ImGuiIsWindowActive()) {
				ImGuiSetActiveWidgetID(id);
				ImGuiState().DoesWindowConsumeTextInput = true;

				caretStart = Max((pvint)val.size() - numCharSlots + 1, 0);	//Start
				caretPos = caretStart + Min(numCharSlots - 1, (int)val.size()); //Position
				heldKeyCooldown = 0.0f;
			}
		}

		if (ImGuiIsWidgetActive(id)) {
			colorBorder = COLOR_WHITE;

			int caretDelta = 0;

			// returned
			if (ImGuiDidKeyJustGoDown(KEY_ENTER)) {
				//ImGuiSetActiveWidgetId(ImGuiManager::State::WIDGET_NULL);
				return 1;
			}

			// did user type a key?
			std::vector<pvchar> cb = ImGuiGetKeyPressed();

			for (char & ch : cb) {
				val.insert(caretPos, ch);
				caretDelta++;
			}

			{
				// keys that can be held down
				pvushort heldKey = 0;
				const pvushort holdableKeys[] = { KEY_LEFT, KEY_RIGHT, KEY_DELETE, KEY_BACKSPACE };
				for (pvushort i = 0; i < std::size(holdableKeys); ++i) {
					// just pressed down
					if (ImGuiDidKeyJustGoDown(holdableKeys[i])) // firstime press
					{
						heldKeyCooldown = KEYPRESS_COOLDOWN;
						heldKey = holdableKeys[i];
						break;
					} else if (ImGuiIsKeyDown(holdableKeys[i])) // held down
					{
						heldKeyCooldown -= Timer::GetDeltatTime().GetS();

						if (heldKeyCooldown <= 0.0f) {
							heldKeyCooldown = KEYHOLD_COOLDOWN;
							heldKey = holdableKeys[i];
							break;
						}
					}
				}

				if (heldKey) {
					if (heldKey == KEY_LEFT) {
						caretDelta--;
					} else if (heldKey == KEY_RIGHT) {
						caretDelta++;
					} else if (heldKey == KEY_DELETE) {
						if (caretPos < (pvint)val.size()) val.erase(caretPos, 1);
					} else if (heldKey == KEY_BACKSPACE) {
						if (!val.empty() && caretPos - 1 >= 0) {
							val.erase(caretPos - 1, 1);
							--caretDelta;
						}
					}
				} else if (ImGuiDidKeyJustGoDown(KEY_HOME)) {
					caretPos = caretStart = 0;
				} else if (ImGuiDidKeyJustGoDown(KEY_END)) {
					caretStart = Max((pvint)val.size() - numCharSlots + 1, 0); // start
					caretPos = caretStart + Min(numCharSlots - 1, (pvint)val.size()); // position
				}
			}

			// move caret left
			if (caretDelta == -1) {
				caretPos = Max(caretPos - 1, 0);
				if (caretPos < caretStart) caretStart -= 1;

				// handle case where deleting
				if (caretStart > 0 && caretPos >= (int)val.size() && (caretStart - caretPos) < numCharSlots) {
					--caretStart;
				}
			}

			// move caret right
			else if (caretDelta == 1) {
				if ((int)val.size() > numCharSlots) // if str is longer than num slots
				{
					if (caretPos < (int)val.size()) ++caretPos;
				} else if (caretPos < (int)val.size()) {
					++caretPos;
				}

				if (caretPos - caretStart >= numCharSlots) {
					++caretStart;
				}
			}
		}

		// draw rect
		ImGuiColor(colorBar);
		ImGuiDrawRect(pos, dimen);

		// tex position
		Vec2i textPos = pos + Vec2i(FILLBAR_TEXT_BUFFER, -FILLBAR_TEXT_BUFFER);
		int characterStart = 0;
		int characterEnd = Min((int)val.size(), numCharSlots);

		// substr with caret positions
		if (ImGuiIsWidgetActive(id)) {
			characterStart = caretStart;
			characterEnd = caretStart + numCharSlots;
		}

		// draw text
		int numCharsPrint = Min(characterEnd - characterStart, numCharSlots);
		ImGuiColor(COLOR_WHITE);
		ImGuiPrint(val.substr(characterStart, numCharsPrint), textPos);

		// do "text_" blinking
		if (ImGuiIsWidgetActive(id) && fmod(Timer::GetTime().GetS(), 1.0f) < 0.5f) {
			Vec2i underlinePos = textPos + ImGui::FONT_WIDTH * Vec2i(caretPos - caretStart, 0);
			ImGuiPrint("_", underlinePos - Vec2i(0, 1));
		}

		MoveDrawPosNextLine(dimen);
		return 0;
	}

	void ImGui::PropertyEditor() {
		Begin("Property Editor");
		SliderRGB("Window Title", COLOR_WINDOW_TITLE);
		Seperator();
		SliderRGB("Window background", COLOR_WINDOW_BACKGROUND);
		End();
	}

}