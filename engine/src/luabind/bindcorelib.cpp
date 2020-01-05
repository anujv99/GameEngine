#include "bindcorelib.h"

#include <cstring>

#include "luabind.h"
#include "virtualmachine/vmlogger.h"

#include "core/window.h"

namespace prev {

	LUA_LIB_START(Window)
		LUA_LIB_ENTRY("GetWidth", [](lua_State * L) -> int { lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetWidth())); return 1; })
		LUA_LIB_ENTRY("GetHeight", [](lua_State * L) -> int { lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetHeight())); return 1; })
		LUA_LIB_ENTRY("GetTitle", [](lua_State * L) -> int { lua_pushstring(L, Window::Ref().GetTitle().c_str()); return 1; })

		LUA_LIB_ENTRY("SetWidth", [](lua_State * L) -> int { if (lua_gettop(L) != 1 && lua_type(L, -1) != LUA_TNUMBER) { 
			VMLogger::Log("Invalid args"); } 
			else { Window::Ref().SetWidth(static_cast<pvuint>(lua_tointeger(L, -1))); } return 0; })
		LUA_LIB_ENTRY("SetHeight", [](lua_State * L) -> int { if (lua_gettop(L) != 1 && lua_type(L, -1) != LUA_TNUMBER) {
			VMLogger::Log("Invalid args");
			} else { Window::Ref().SetHeight(static_cast<pvuint>(lua_tointeger(L, -1))); } return 0; })
		LUA_LIB_ENTRY("SetTitle", [](lua_State * L) -> int { if (lua_gettop(L) != 1 && lua_type(L, -1) != LUA_TSTRING) {
			VMLogger::Log("Invalid args");
			} else { Window::Ref().SetTitle(lua_tostring(L, -1)); } return 0; })
	LUA_LIB_END(Window)

	struct WindowMetatable {
		static int Index(lua_State * L) {
			pvstring index = lua_tostring(L, -1);
			if (index == "Title") {
				lua_pushstring(L, Window::Ref().GetTitle().c_str()); return 1;
			} else if (index == "Width") {
				lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetWidth())); return 1;
			} else if (index == "Height") {
				lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetHeight())); return 1;
			}

			VMLogger::Log("Invalid index %s", index);
			return 0;
		}

		static int NewIndex(lua_State * L) {
			auto a = lua_gettop(L);
			auto b = lua_type(L, -1);
			auto c = lua_type(L, -2);
			auto d = lua_type(L, -3);

			pvstring index = lua_tostring(L, -2);

			if (index == "Title") {
				if (lua_type(L, -1) == LUA_TSTRING) {
					Window::Ref().SetTitle(lua_tostring(L, -1));
					return 0;
				}
			} else if (index == "Width") {
				if (lua_type(L, -1) == LUA_TNUMBER) {
					Window::Ref().SetWidth(static_cast<pvuint>(lua_tointeger(L, -1)));
					return 0;
				}
			} else if (index == "Height") {
				if (lua_type(L, -1) == LUA_TNUMBER) {
					Window::Ref().SetHeight(static_cast<pvuint>(lua_tointeger(L, -1)));
					return 0;
				}
			}

			VMLogger::Log("Invalid index '%s' or incompatible argument", index.c_str());
			return 0;
		}
	};

	LUA_METATABLE_START(Window)
		LUA_METATABLE_FUNCTION("__index", WindowMetatable::Index)
		LUA_METATABLE_FUNCTION("__newindex", WindowMetatable::NewIndex)
	LUA_METATBLE_END(Window)

	void LuaBindCoreLib(lua_State * L) {
		// ----------- Window Lib -----------
		LUA_REGISTER_LIB(Window);
		LUA_SET_METATABLE(Window);
	}

}
