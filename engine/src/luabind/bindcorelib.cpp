#include "bindcorelib.h"

#include "luabind.h"
#include "virtualmachine/lua.hpp"

#include "core/window.h"

namespace prev {

	LUA_LIB_START(Window)
		LUA_LIB_ENTRY("GetWidth", [](lua_State * L) -> int { lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetWidth())); return 1; }),
		LUA_LIB_ENTRY("GetHeight", [](lua_State * L) -> int { lua_pushinteger(L, static_cast<lua_Integer>(Window::Ref().GetHeight())); return 1; }),
		LUA_LIB_ENTRY("GetTitle", [](lua_State * L) -> int { lua_pushstring(L, Window::Ref().GetTitle().c_str()); return 1; }),
	LUA_LIB_END(Window)

	void LuaBindCoreLib(lua_State * L) {
		// ----------- Window Lib -----------
		LUA_REGISTER_LIB(Window);
	}

}
