#include "virtualmachine.h"

#include "utils/datafile.h"

#include "luabind/bindcorelib.h"

namespace prev {

	VirtualMachine::VirtualMachine() : L(nullptr) {
		L = luaL_newstate();
		luaL_openlibs(L);

		LuaBindCoreLib(L);
	}

	VirtualMachine::~VirtualMachine() {
		lua_close(L);
	}

	void VirtualMachine::DoString(const pvstring & script) {
		if (luaL_dostring(L, script.c_str()) != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
			lua_pop(L, 1);
			return;
		}
	}

	Config VirtualMachine::ReadConfigFile(const pvstring & path) {
		DataFile file;
		if (!file.OpenFile(path)) { return Config(); }

		DoString(file.ReadEntireFile());

		Config c;

		lua_getglobal(L, "Config");
		if (lua_type(L, -1) != LUA_TTABLE) { LOG_ERROR("Invalid config file : %s", path.c_str()); return c; }

		lua_pushstring(L, "Width");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TNUMBER) { c.WindowWidth = lua_tointeger(L, -1); lua_pop(L, 1); }

		lua_pushstring(L, "Height");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TNUMBER) { c.WindowHeight = lua_tointeger(L, -1); lua_pop(L, 1); }

		lua_pushstring(L, "Title");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TSTRING) { c.WindowTitle = lua_tostring(L, -1); lua_pop(L, 1); }

		lua_pushstring(L, "API");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TSTRING) { c.GraphicsAPI = lua_tostring(L, -1); lua_pop(L, 1); }
		
		lua_pop(L, 1);

		return c;
	}

}