#pragma once

#include "../common/types.h"
#include "../virtualmachine/lua.hpp"

namespace prev {

	struct LuaFunction {
		const pvchar * Name;
		pvint(*Func)(lua_State * L);
	};

}

#define LUA_LIB_START(N) static prev::LuaFunction Lua##N##LibEntry[] = {
#define LUA_LIB_ENTRY(N, F) { N, F },
#define LUA_LIB_END(N) };

#define LUA_REGISTER_LIB(N)\
	lua_newtable(L);\
	for (prev::pvsizet i = 0; i < std::size(prev::Lua##N##LibEntry); i++) {\
		lua_pushstring(L, prev::Lua##N##LibEntry[i].Name);\
		lua_pushcfunction(L,  prev::Lua##N##LibEntry[i].Func);\
		lua_settable(L, -3);\
	}\
	lua_setglobal(L, #N);

#define LUA_METATABLE_START(N) static prev::LuaFunction Lua##N##MetatableFunctions[] = {
#define LUA_METATABLE_FUNCTION(N, F) { N, F },
#define LUA_METATBLE_END(N) };

#define LUA_SET_METATABLE(N)\
	lua_getglobal(L, #N);\
	if (lua_type(L, -1) != LUA_TTABLE) {\
		LOG_ERROR("Not library with name %s found", #N);\
	} else {\
		luaL_newmetatable(L, #N "Metatable");\
		for (prev::pvsizet i = 0; i < std::size(prev::Lua##N##MetatableFunctions); i++) {\
			lua_pushstring(L, prev::Lua##N##MetatableFunctions[i].Name);\
			lua_pushcfunction(L, Lua##N##MetatableFunctions[i].Func);\
			lua_settable(L, -3);\
		}\
		lua_setmetatable(L, -2);\
	}