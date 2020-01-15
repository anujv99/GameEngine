#pragma once

#include <unordered_map>
#include <vector>
#include <set>

#include "../common/singleton.h"
#include "../common/handledobject.h"
#include "../graphics/font.h"
#include "../math/math.h"

#include "ImGuiwindow.h"

namespace prev {

	class ImGuiManager : public Singleton<ImGuiManager> {
		friend class Singleton<ImGuiManager>;
	private:
		ImGuiManager();
		~ImGuiManager();
	public:
		struct ImGuiStateStruct {
			union {
				pvfloat				F;
				pvint				I;
				pvuint				UInt;
				struct { pvfloat	FVec[4]; };
				struct { pvint		IVec[4]; };
			} Data;

			Vec2i DrawPos;
			Vec2i DrawPosPrev;
			Vec2i ScrollOffset;

			pvint NumTabs;

			Vec2i MouseRegionStart;
			Vec2i MouseRegionDimen;

			static constexpr const int WIDGET_NULL		= -1;
			static constexpr const int WIDGET_WINDOW	=  1;

			pvint ActiveWidgetID;
			pvint WidgetCount;
			pvbool DoesWindowConsumeTextInput;

			StrongHandle<ImGuiWindow> ActiveWindow;
			StrongHandle<ImGuiWindow> WorkingWindow;

			void Reset();
		} State;

		void PreUpdate();
		void PostUpdate();
		void ClearActiveWindow();
		void ClearAllWindow();
		void OnResizeScreen(Vec2 prevDimen, Vec2 newDimen);

		void SetActiveWindow(StrongHandle<ImGuiWindow> window);
		void SetActiveWidgetID(pvint id);
		void BringWindowToFront(StrongHandle<ImGuiWindow> window);

		pvint GetWindowOrderIndex(pvstring name) const;
		pvbool IsPosInWindow(Vec2i pos, StrongHandle<ImGuiWindow> window) const;

		void DetectConsumeInputs();
		pvbool DidMouseJustGoDown(pvint button) const;
		pvbool DidKeyJustGoDown(pvint keyCode) const;

		StrongHandle<ImGuiWindow> GetWindow(pvstring name);

		inline StrongHandle<Font> GetFont() { return m_Font; }
		inline std::unordered_map<pvuint, StrongHandle<ImGuiWindow>> & GetWindowList() { return m_WindowMap; }
	private:
		void PruneVisibleWindow();
		void UpdateWindowMap();
		void DebugPrintVisibleWindow();
		void DrawWindows();
	private:
		std::unordered_map<pvuint, StrongHandle<ImGuiWindow>> m_WindowMap;
		std::vector<StrongHandle<ImGuiWindow>> m_VisibleWindows;
		// [TODO] Set window draw calls and setup rendering

		StrongHandle<Font> m_Font;
		pvfloat m_FontScale;

		std::set<pvint> m_ConsumeMouseButtons;
		std::set<pvchar> m_ConsumeKeyPress;
	private:
		friend void ImGuiColor(Vec3, pvfloat);
		friend void ImGuiDrawRect(Vec2i, Vec2i);
		friend void ImGuiPrint(const pvstring &, Vec2i);
		friend void ImGuiDrawLine(Vec2i, Vec2i);
		struct WindowDrawCall : public HandledObject<WindowDrawCall> {
			std::vector<Vec2> QuadPos;
			std::vector<Vec2> QuadDimen;
			std::vector<Vec4> QuadColor;

			std::vector<pvstring> Text;
			std::vector<Vec2> TextPos;
			std::vector<Vec4> TextColor;
		};
		std::unordered_map<uint32_t, StrongHandle<WindowDrawCall>> m_WindowDrawCalls;
		StrongHandle<WindowDrawCall> GetDrawCall(StrongHandle<ImGuiWindow> window);

		Vec4 m_CurrentColor;
	};

}