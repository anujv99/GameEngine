#include "ImGuimanager.h"

#include "core/window.h"
#include "core/input.h"

#include "ImGui.h"

#include "renderer/renderer2d.h"

namespace prev {

	static constexpr const pvuint DEAD_WINDOW_NUM_FRAMES = 100000;

	ImGuiManager::ImGuiManager() {
		m_Font = new Font("../../../../res/fonts/Minecraftia.ttf", 8);
		m_FontScale = 10.0f;
		State.Reset();

		Vec2i windowSize = Vec2i(Window::Ref().GetWidth(), Window::Ref().GetHeight());
		State.MouseRegionStart = Vec2i(0, windowSize.y);
		State.MouseRegionDimen = windowSize;

		ImGui::FONT_WIDTH = m_Font->GetSize("U").x * m_FontScale;
		ImGui::FONT_HEIGHT = m_Font->GetSize("U").y * m_FontScale;
	}

	ImGuiManager::~ImGuiManager() {}

	void ImGuiManager::ImGuiStateStruct::Reset() {
		ActiveWindow					= nullptr;
		WorkingWindow					= nullptr;
		ActiveWidgetID					= ImGuiStateStruct::WIDGET_NULL;
		WidgetCount						= 0;
		DoesWindowConsumeTextInput		= false;

		memset(&Data, 0, sizeof(Data));
	}

	void ImGuiManager::PreUpdate() {
		PruneVisibleWindow();
		UpdateWindowMap();
	}

	void ImGuiManager::PostUpdate() {
		DrawWindows();

		m_ConsumeKeyPress.clear();
		m_ConsumeMouseButtons.clear();
	}

	void ImGuiManager::ClearActiveWindow() {
		State.ActiveWindow					= nullptr;
		State.WidgetCount					= 0;
		State.DoesWindowConsumeTextInput	= false;
	}

	void ImGuiManager::ClearAllWindow() {
		ClearActiveWindow();
		State.Reset();
		m_WindowMap.clear();
		m_VisibleWindows.clear();
	}

	void ImGuiManager::OnResizeScreen(Vec2 prevDimen, Vec2 newDimen) {
		for (auto & window : m_WindowMap) {
			pvfloat prevDistFromTop = prevDimen.y - window.second->Pos.y;
			pvfloat newY = newDimen.y - prevDistFromTop;
			window.second->Pos.y = newY;
		}
	}

	void ImGuiManager::SetActiveWindow(StrongHandle<ImGuiWindow> window) {
		if (window == nullptr) {
			ClearActiveWindow();
		}
		if (State.ActiveWindow != window) {
			SetActiveWidgetID(ImGuiStateStruct::WIDGET_NULL);
		}
		if (window != nullptr && window != State.ActiveWindow) {
			BringWindowToFront(window);
		}
		State.ActiveWindow = window;
	}

	void ImGuiManager::SetActiveWidgetID(pvint id) {
		State.ActiveWidgetID = id;
		State.DoesWindowConsumeTextInput = false;
	}

	void ImGuiManager::BringWindowToFront(StrongHandle<ImGuiWindow> window) {
		if (window == nullptr) return;
		pvint index = -1;
		for (pvsizet i = 0; i < m_VisibleWindows.size(); i++) {
			if (m_VisibleWindows[i] == window) {
				index = i;
				break;
			}
		}

		if (index < 0) return;

		auto win = m_VisibleWindows[index];

		for (pvsizet i = index; i > 0; i--) {
			m_VisibleWindows[i] = m_VisibleWindows[i - 1];
		}

		m_VisibleWindows[0] = win;
	}

	pvint ImGuiManager::GetWindowOrderIndex(pvstring name) const {
		pvuint hash = HashString(name);
		const auto it = m_WindowMap.find(hash);
		if (it == m_WindowMap.end()) return -1;

		for (pvsizet i = 0; i < m_VisibleWindows.size(); i++) {
			const auto window = m_VisibleWindows[i];
			if (window == it->second) {
				return static_cast<pvint>(i);
			}
		}

		return -1;
	}

	pvbool ImGuiManager::IsPosInWindow(Vec2i pos, StrongHandle<ImGuiWindow> window) const {
		if (window == nullptr) return false;

		Vec2i windowPos = window->Pos;
		Vec2i windowDimen = window->Dimen;

		if (window->IsMinimized || window->IsLocked) {
			windowDimen.y = ImGui::TITLE_BAR_HEIGHT;
		}

		bool posInWindow = pos.x >= windowPos.x && pos.x <= windowPos.x + windowDimen.x &&
						   pos.y <= windowPos.y && pos.y >= windowPos.y - windowDimen.y;

		return posInWindow;
	}

	void ImGuiManager::DetectConsumeInputs() {
		const pvint minMouse = MOUSE_BUTTON_1;
		const pvint maxMouse = MOUSE_BUTTON_5;
		const Vec2i mousePos = Vec2i(Input::GetMousePosition().x, Window::Ref().GetHeight() - Input::GetMousePosition().y);

		pvbool mouseClicked = false;
		pvbool mouseOverAnyWindow = false;

		for (auto & window : m_VisibleWindows) {
			// Is mouse in
			const pvbool mouseInWindow = IsPosInWindow(mousePos, window);
			mouseOverAnyWindow |= mouseInWindow;

			// Check all mouse button presses if is in
			for (pvint i = minMouse; i < maxMouse; i++) {
				if (Input::IsMouseButtonPressed(i)) {
					mouseClicked = true;

					// If did not click on active window
					auto activeWindow = State.ActiveWindow;
					if (activeWindow == nullptr || (mouseInWindow && window != activeWindow && !IsPosInWindow(mousePos, activeWindow))) {
						SetActiveWindow(window);
					}
					if (mouseInWindow) {
						m_ConsumeMouseButtons.insert(i);
					}
				}
			}
		}

		// If clicked but missed all windows
		if (mouseClicked && !mouseOverAnyWindow) {
			ClearActiveWindow();
		}

		if (State.DoesWindowConsumeTextInput) {
			const std::vector<pvchar> & cb = Input::GetPressedCharacterBuffer();

			for (auto & c : cb) {
				m_ConsumeKeyPress.insert(c);
			}
		}
	}

	pvbool ImGuiManager::DidMouseJustGoDown(pvint button) const {
		return Input::IsMouseButtonPressed(button);
	}

	pvbool ImGuiManager::DidKeyJustGoDown(pvint keyCode) const {
		return Input::IsKeyPressed(keyCode);
	}

	StrongHandle<ImGuiWindow> ImGuiManager::GetWindow(pvstring name) {
		pvuint hash = HashString(name);
		const auto it = m_WindowMap.find(hash);

		if (it == m_WindowMap.end()) {
			StrongHandle<ImGuiWindow> result = new ImGuiWindow();
			result->Name = name;
			m_WindowMap[hash] = result;
			return result;
		}

		return it->second;
	}

	void ImGuiManager::PruneVisibleWindow() {
		for (pvsizet i = 0; i < m_VisibleWindows.size(); i++) {
			auto window = m_VisibleWindows[i];

			if (window->FramesSinceUpdate > 0) {
				m_VisibleWindows[i] = m_VisibleWindows.back();
				m_VisibleWindows.resize(m_VisibleWindows.size() - 1);
			}
		}
	}

	void ImGuiManager::UpdateWindowMap() {
		auto it = m_WindowMap.begin();

		while (it != m_WindowMap.end()) {
			auto window = it->second;

			if (window->FramesSinceUpdate > 1) {
				if (State.ActiveWindow == window) {
					ClearActiveWindow();
				}
				if (window->FramesSinceUpdate > DEAD_WINDOW_NUM_FRAMES) {
					m_WindowMap.erase(it++);
					continue;
				}
			} else {
				bool isInVisibleList = false;
				for (auto visibleIt : m_VisibleWindows) {
					if (visibleIt == window) {
						isInVisibleList = true;
						break;
					}
				}
				if (!isInVisibleList) {
					m_VisibleWindows.push_back(window);
				}
			}

			(window->FramesSinceUpdate)++;
			it++;
		}
	}

	void ImGuiManager::DebugPrintVisibleWindow() {
		static pvint frame = 0;
		LOG_TRACE("------------[%d]------------", frame++);
		for (pvsizet i = 0; i < m_VisibleWindows.size(); i++) {
			auto window = m_VisibleWindows[i];
			LOG_TRACE("[%d] %s - frameSinceUpdate : %d", i, window->Name.c_str(), window->FramesSinceUpdate);
		}
	}

	void ImGuiManager::DrawWindows() {
		for (auto win = m_VisibleWindows.rbegin(); win != m_VisibleWindows.rend(); win++) {
			auto drawCall = GetDrawCall(win->Get());
			if (drawCall == nullptr) continue;

			for (pvsizet i = 0; i < drawCall->QuadPos.size(); i++) {
				Renderer2D::Ref().DrawSprite(drawCall->QuadPos[i], drawCall->QuadDimen[i], drawCall->QuadColor[i], nullptr);
			}
			drawCall->QuadPos.resize(0);
			drawCall->QuadDimen.resize(0);
			drawCall->QuadColor.resize(0);

			for (pvsizet i = 0; i < drawCall->TextPos.size(); i++) {
				Label l;
				l.Pos = drawCall->TextPos[i];
				l.Color = drawCall->TextColor[i];
				l.Scale = Vec2(m_FontScale);
				l.Text = drawCall->Text[i];
				l.Align = Label::Alignment::RIGHT;

				Renderer2D::Ref().DrawText(m_Font, l);
			}
			drawCall->Text.resize(0);
			drawCall->TextColor.resize(0);
			drawCall->TextPos.resize(0);
		}

		for (auto win : m_WindowDrawCalls) {
			win.second->QuadPos.resize(0);
			win.second->QuadDimen.resize(0);
			win.second->QuadColor.resize(0);

			win.second->Text.resize(0);
			win.second->TextPos.resize(0);
			win.second->TextColor.resize(0);
		}
	}

	StrongHandle<ImGuiManager::WindowDrawCall> ImGuiManager::GetDrawCall(StrongHandle<ImGuiWindow> window) {
		ASSERTM(window != nullptr, "Invalid ImGui window");

		uint32_t hash = HashString(window->Name);

		auto it = m_WindowDrawCalls.find(hash);
		if (it != m_WindowDrawCalls.end()) {
			return it->second;
		}
		m_WindowDrawCalls[hash] = new WindowDrawCall();
		return m_WindowDrawCalls.find(hash)->second;
	}



}