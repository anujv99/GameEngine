#include "virtualmachine.h"
#include "vmlogger.h"

#include "utils/datafile.h"

#include "luabind/bindcorelib.h"
#include "luabind/bindimguilib.h"

namespace prev {

	static lua_State * S;

	VirtualMachine::VirtualMachine() : L(nullptr), m_GuiFunction(-1), m_RenderFunction(-1), m_UpdateFunction(-1) {
		L = luaL_newstate();
		luaL_openlibs(L);

		LuaBindCoreLib(L);
		LuaBindImGuiLib(L);
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
		
		lua_pushstring(L, "ResDir");
		lua_gettable(L, -2);
		if (lua_type(L, -1) == LUA_TSTRING) { c.ResFolderDir = lua_tostring(L, -1); lua_pop(L, 1); }

		lua_pop(L, 1);

		return c;
	}

	void VirtualMachine::Init(const pvstring & script) {
		if (!FileUtils::FileExists(script)) { LOG_ERROR("Unable to find main script file"); return; }

		if (luaL_dofile(L, script.c_str()) != LUA_OK) {
			VMLogger::Log("%s", lua_tostring(L, -1));
		}

		lua_getglobal(L, "g_main_app");
		ASSERTM(lua_type(L, -1) == LUA_TTABLE, "Invalid script file");

		lua_pushstring(L, "update");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_app table should contain update function");
		m_UpdateFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pushstring(L, "render");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_app table should contain render function");
		m_RenderFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pushstring(L, "gui");
		lua_gettable(L, -2);
		ASSERTM(lua_type(L, -1) == LUA_TFUNCTION, "main_app table should contain gui function");
		m_GuiFunction = luaL_ref(L, LUA_REGISTRYINDEX);

		lua_pop(L, 1);
	}

	void VirtualMachine::Update(float dt) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_UpdateFunction);
		lua_getglobal(L, "g_main_app");
		lua_pushnumber(L, dt);
		int err = lua_pcall(L, 2, 0, 0);
		if (err != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

	void VirtualMachine::Render() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_RenderFunction);
		lua_getglobal(L, "g_main_app");
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

	void VirtualMachine::Gui() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_GuiFunction);
		lua_getglobal(L, "g_main_app");
		int err = lua_pcall(L, 1, 0, 0);
		if (err != LUA_OK) {
			LOG_ERROR("LUA ERROR : %s", lua_tostring(L, -1));
		}
	}

}