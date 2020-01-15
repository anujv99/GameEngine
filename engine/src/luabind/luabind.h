#pragma once

#include "../common/types.h"
#include "../virtualmachine/lua.hpp"
#include "../virtualmachine/vmlogger.h"

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


// ****************************************************************************************************** //

#define LUA_CHECK_NUM_PARAMS(N) if (lua_gettop(L) != N) { prev::VMLogger::Log("Expected %d number of params but got %d", N, lua_gettop(L));\
								return 0; }

#define LUA_FLOAT_PARAM(P, V) if (lua_type(L, P) != LUA_TNUMBER) { prev::VMLogger::Log("Expected integer as parameter at index %d", P); return 0; }\
								prev::pvfloat V = lua_tonumber(L, P)
#define LUA_INT_PARAM(P, V) if (lua_type(L, P) != LUA_TNUMBER) { prev::VMLogger::Log("Expected integer as parameter at index %d", P); return 0; }\
								prev::pvint V = lua_tointeger(L, P)
#define LUA_STRING_PARAM(P, V) if (lua_type(L, P) != LUA_TSTRING) { prev::VMLogger::Log("Expected string as parameter at index %d", P); return 0; }\
								const prev::pvchar * V = lua_tostring(L, P)
