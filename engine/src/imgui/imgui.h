#pragma once

#include "../common/types.h"
#include "../math/math.h"
#include "../common/stronghandle.h"

namespace prev {

	struct ImGui {
		static const pvint AUTOPOS  = -1;
		static const pvint AUTOSIZE = -1;

		static void Begin(pvstring name, Vec2i pos = Vec2i(AUTOPOS), Vec2i dimen = Vec2i(AUTOSIZE));
		static void End();



		static pvint FONT_WIDTH;
		static pvint FONT_HEIGHT;
		static pvint TITLE_BAR_HEIGHT;

		static void MoveDrawPosBy(Vec2i dimen);
		static void MoveDrawPosNextLine(Vec2i dimen);
	};

}