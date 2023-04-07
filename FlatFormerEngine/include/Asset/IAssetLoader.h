#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Asset {

#pragma region Forward declaration
	class AssetHandle;
#pragma endregion

	// Some resources must be processed / transformed when loaded in a certain way,
	// so for each such type of resource, a loader is defined
	class IAssetLoader
	{
	public:
		// Allows you to find out what type of files the loader is processing
		virtual std::string GetPattern() = 0;
	
		// Returns true if the loader can use the file in its pure form and extra data processing is not required
		virtual bool IsUseRawFile() = 0;
	
		// (if the raw buffer is not needed after loading, it should not take up memory, so we free it)
		virtual bool IsDiscardRawBufferAfterLoad() = 0;
	
		// Get the size of the loaded resource
		virtual unsigned int GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	
		// Method for loading a resource from a file
		virtual bool LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<AssetHandle> handle) = 0;
	};
}}