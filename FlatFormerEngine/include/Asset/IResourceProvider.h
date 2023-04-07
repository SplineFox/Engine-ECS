#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Asset {

#pragma region Forward declaration
	class Asset;
#pragma endregion

	class IResourceProvider
	{
	public:
		virtual ~IResourceProvider() { }

		// Opens file and returns true / false
		// depending on the existence and integrity of the file
		virtual bool Open() = 0;

		virtual int GetRawResourceSize(const Asset &asset) = 0;

		// Gets the contents of a file
		virtual int GetRawResource(const Asset &asset, char *buffer) = 0;

		// Get the number of resources inside the file (if it is an archive)
		virtual int GetNumResources() const = 0;

		// Getting the name of a resource by its index inside a file
		virtual std::string GetResourceName(int assetID) const = 0;

		// Gets a flag for whether AssetFile uses zip or reads resources from a directory
		virtual bool IsUsingDevelopmentDirectories() const = 0;
	};

}}