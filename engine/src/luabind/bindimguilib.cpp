#include "bindimguilib.h"

#include "luabind.h"
#include "virtualmachine/vmlogger.h"

#include "imgui/imgui.h"

namespace prev {

	struct LuaImGuiFunc {
		static int Begin(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(1);
			LUA_STRING_PARAM(1, name);
			ImGui::Begin(name);
			return 0;
		}

		static int TextInput(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(3);
			LUA_STRING_PARAM(1, name);
			LUA_STRING_PARAM(2, text);
			LUA_INT_PARAM(3, width);

			pvstring str(text);

			ImGui::TextInput(name, str, width);

			lua_pushstring(L, str.c_str());
			return 1;
		}

		static int End(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			ImGui::End();
			return 0;
		}

		static int SliderFloat(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(4);
			LUA_STRING_PARAM(1, name);
			LUA_FLOAT_PARAM(2, val);
			LUA_FLOAT_PARAM(3, min);
			LUA_FLOAT_PARAM(4, max);
			lua_pushnumber(L, ImGui::SliderFloat(name, val, min, max));
			return 1;
		}

		static int Seperator(lua_State * L) {
			LUA_CHECK_NUM_PARAMS(0);
			ImGui::Seperator();
			return 0;
		}
	};

	LUA_LIB_START(ImGui)
		LUA_LIB_ENTRY("Begin", LuaImGuiFunc::Begin)
		LUA_LIB_ENTRY("End", LuaImGuiFunc::End)
		LUA_LIB_ENTRY("TextInput", LuaImGuiFunc::TextInput)
		LUA_LIB_ENTRY("SliderFloat", LuaImGuiFunc::SliderFloat)
		LUA_LIB_ENTRY("Seperator", LuaImGuiFunc::Seperator)
	LUA_LIB_END(ImGui)

	void LuaBindImGuiLib(lua_State * L) {
		LUA_REGISTER_LIB(ImGui);
	}
}