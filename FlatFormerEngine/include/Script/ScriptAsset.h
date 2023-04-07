#pragma once

#include "Asset/IAssetLoader.h"
#include "Script/LuaInteractionLayer.h"

namespace FlatFormer {

	class ScriptAssetPayloadData : public Asset::IAssetPayloadData
	{
		friend class ScriptAssetLoader;

	protected:
		// This is unique_ptr because LuaRef does not have a default constructor.
		// The memory for chunkFunctionPtr will be allocated as soon as the chunk is loaded from the script.
		std::unique_ptr<luabridge::LuaRef> chunkFunctionPtr;

	public:
		ScriptAssetPayloadData()
		{}

		~ScriptAssetPayloadData() 
		{}

		std::string ToString() override { return "ScriptAssetPayloadData"; }
	};



	class ScriptAssetLoader : public Asset::IAssetLoader
	{
	public:
		std::string GetPattern() override
		{ return "*.lua"; }

		bool IsUseRawFile() override 
		{ return false; }

		bool IsDiscardRawBufferAfterLoad() override
		{ return true; }

		unsigned int GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) override
		{ return rawSize; }


		bool LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<Asset::AssetHandle> handle) override
		{
			if (rawSize <= 0)
				return false;

			/// create payload to assign handle
			std::shared_ptr<ScriptAssetPayloadData> payload = std::shared_ptr<ScriptAssetPayloadData>(new ScriptAssetPayloadData());
			/// load the Lua script as a string and initialize the pointer
			payload->chunkFunctionPtr = std::make_unique<luabridge::LuaRef>(Script::LuaInteractionLayer::Instance().LoadLuaString(rawBuffer));
			/// if chunkFunctionPtr is a function pointer, then loading was successful
			if (payload->chunkFunctionPtr->isFunction())
			{
				handle->SetPayload(std::shared_ptr<ScriptAssetPayloadData>(payload));
				return true;
			}
			return false;
		}

	};

}