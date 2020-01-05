#pragma once

#include "../math/math.h"
#include "../common/handledobject.h"
#include "../common/types.h"

namespace prev {

	struct ImGuiWindow : public HandledObject<ImGuiWindow> {
		pvstring Name;					//	Name

		Vec2i Pos;						//	Center Position
		Vec2i Dimen;					//	Dimension of window
		Vec2i DimenAutoSize;			//	Autosize
		Vec2i DimenAutoSizePrev;		//	Autosize of last frame
		Vec2i ScrollPos;				//	Scroll position
		
		pvint TitleBarMinWidth;			//	Minimum title bar width;
		pvfloat BackgroundAlpha;		//	Background alpha

		pvbool AutoSize;				//	Autosize on/off
		pvbool IsMinimized;				//	Is minimized
		pvbool IsNewWindow;				//	True for new window
		pvbool IsLocked;				//	Lock window on/off

		pvint FramesSinceUpdate;		//	>=1 means window is not active

		ImGuiWindow();
	};

}