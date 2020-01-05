#include "imguiwindow.h"

namespace prev {

	ImGuiWindow::ImGuiWindow() :
		Pos(-1),
		Dimen(1),
		DimenAutoSize(0),
		DimenAutoSizePrev(0),
		ScrollPos(0),
		TitleBarMinWidth(0),
		BackgroundAlpha(0.5f),
		AutoSize(false),
		IsMinimized(false),
		IsNewWindow(true),
		IsLocked(false),
		FramesSinceUpdate(0)
	{}

}