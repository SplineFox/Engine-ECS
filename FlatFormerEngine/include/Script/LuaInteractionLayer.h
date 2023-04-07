#pragma once

#include "Engine/API.h"

#include "Script/LuaMetamethods.h"

extern "C"
{
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}
///note: all lua files must be connected before LuaBridge
#include "LuaBridge/LuaBridge.h"


namespace FlatFormer { namespace Script {

	class LuaInteractionLayer
	{
#pragma region Singleton
	public:
		static LuaInteractionLayer& Instance();

	private:
		LuaInteractionLayer();
		~LuaInteractionLayer();

		LuaInteractionLayer(const LuaInteractionLayer&) = delete;
		LuaInteractionLayer& operator= (const LuaInteractionLayer&) = delete;
#pragma endregion


	private:
		lua_State* lua_state;
		FFE_DECLARE_STATIC_LOGGER

	public:
		luabridge::LuaRef LoadLuaString(const char* str);
		luabridge::LuaRef LoadLuaFile(const char* file);

		std::unordered_map<std::string, luabridge::LuaRef> GetTableMap(const luabridge::LuaRef& tableRef);
		lua_State* GetState() const;

		void ClearStack();
		void PrintStack();
		void PrintLuaTable(const char* tableName);
		void PrintGlobalTable();
	};

}}

