#include "Script/LuaInteractionLayer.h"

namespace FlatFormer { namespace Script {

	FFE_DEFINE_STATIC_LOGGER(LuaInteractionLayer, "LuaInteractionLayer")

#pragma region Singleton
	LuaInteractionLayer& LuaInteractionLayer::Instance()
	{
		static LuaInteractionLayer s_Instance;
		return s_Instance;
	}


	LuaInteractionLayer::LuaInteractionLayer()
	{
		lua_state = luaL_newstate();
		luaL_openlibs(lua_state);

		FFE_LOG_INFO("Initialized")
	}


	LuaInteractionLayer::~LuaInteractionLayer()
	{
		if (lua_state)
			lua_close(lua_state);

		FFE_LOG_INFO("Released")
	}
#pragma endregion

	// Load Lua string. Upon successful loading,
	// a pointer to the function of the compiled chunk is returned,
	// otherwise Nil pointer
	luabridge::LuaRef FlatFormer::Script::LuaInteractionLayer::LoadLuaString(const char * str)
	{
		/// loads the string like a Lua chunk, without executing
		luaL_loadstring(lua_state, str);
		/// getting a pointer to a function of a compiled chunk
		luabridge::LuaRef loadResult = luabridge::LuaRef::fromStack(lua_state, -1);
		/// if the result is a function, then the download was successful
		if (loadResult.isFunction())
		{
			return loadResult;
		}
		/// otherwise, the result will be an error message
		else
		{
			FFE_LOG_ERROR("LoadLuaString: {}", loadResult.cast<std::string>());
			loadResult = luabridge::Nil();
			return loadResult;
		}
	}


	// Загрузка Lua файла.
	// При успешной загрузке возвращается указатель
	// на функцию скомпилированного chunk'а, иначе
	// Nil указатель
	luabridge::LuaRef FlatFormer::Script::LuaInteractionLayer::LoadLuaFile(const char * fileName)
	{
		/// loads the string like a Lua chunk, without executing
		luaL_loadstring(lua_state, fileName);
		/// getting a pointer to a function of a compiled chunk
		luabridge::LuaRef loadResult = luabridge::LuaRef::fromStack(lua_state, -1);
		/// if the result is a function, then the download was successful
		if (loadResult.isFunction())
		{
			return loadResult;
		}
		/// otherwise, the result will be an error message
		else
		{
			FFE_LOG_ERROR("LoadLuaFile: {}", loadResult.cast<std::string>());
			loadResult = luabridge::Nil();
			return loadResult;
		}
	}


	void LuaInteractionLayer::ClearStack()
	{
		lua_settop(lua_state, 0);
	}


	void LuaInteractionLayer::PrintStack() 
	{
		printf("\n---------------\n");
		int i;
		int top = lua_gettop(lua_state);	// depth of the stack
		if (top > 0)
		{
			for (i = 1; i <= top; i++)			// repeat for each level
			{
				int t = lua_type(lua_state, i);
				switch (t) {
				case LUA_TSTRING: {				// strings
					printf("[%d] ", i);
					printf("'%s'", lua_tostring(lua_state, i));
					break;
				}
				case LUA_TBOOLEAN: {			// booleans
					printf("[%d] ", i);
					printf(lua_toboolean(lua_state, i) ? "true" : "false");
					break;
				}
				case LUA_TNUMBER: {				// numbers
					printf("[%d] ", i);
					printf("%g", lua_tonumber(lua_state, i));
					break;
				}
				default: {						// other values
					printf("[%d] ", i);
					printf("%s", lua_typename(lua_state, t));
					break;
				}
				}
				printf("\n");				// put a separator
			}
		}
		else
		{
			printf("STACK IS EMPTY!\n");
		}
		printf("---------------\n\n");
	}


	// Print Lua table by given name
	void LuaInteractionLayer::PrintLuaTable(const char* tableName)
	{
		printf("\n---------------\n");
		printf("[%s]", tableName);
		printf("\n---------------\n");
		luabridge::LuaRef table = luabridge::getGlobal(lua_state, tableName);
		for (auto& pair : GetTableMap(table))
		{
			printf("%s\n", pair.first.c_str());
		}
		printf("---------------\n\n");
	}


	// Print Lua global table
	void LuaInteractionLayer::PrintGlobalTable()
	{
		PrintLuaTable("_G");
	}


	std::unordered_map<std::string, luabridge::LuaRef> LuaInteractionLayer::GetTableMap(const luabridge::LuaRef& tableRef)
	{
		using namespace luabridge;
		std::unordered_map<std::string, LuaRef> map;
		if (tableRef.isNil()) { return map; }

		auto L = tableRef.state();
		push(L, tableRef);	/// push the table onto the stack

		lua_pushnil(L);	/// push nil onto the stack so that lua_next will pop it and push (k, v) onto the stack
		while (lua_next(L, -2) != 0) /// -2 because table be at -1
		{
			if (lua_isstring(L, -2)) /// we are only interested in string keys
			{
				map.emplace(lua_tostring(L, -2), LuaRef::fromStack(L, -1));
			}
			lua_pop(L, 1); /// remove the value so that lua_next continues to work
		}

		lua_pop(L, 1); /// remove the table
		return map;
	}


	lua_State* FlatFormer::Script::LuaInteractionLayer::GetState() const
	{
		return lua_state;
	}

}}