#pragma once

#include "Engine/API.h"

#include "Asset/Asset.h"
#include "Asset/IResourceProvider.h"

#include "Asset/Render/JPGAssetLoader.h"
#include "Asset/Render/PNGAssetLoader.h"
#include "Asset/Render/TTFAssetLoader.h"
#include "Asset/DefaultAssetLoader.h"

namespace FlatFormer{ namespace Asset {

#pragma region MyRegion
	// contains the correspondence of file names and their positions inside the Zip
	using AssetHandleList = std::list	<std::shared_ptr <AssetHandle>>;
	using AssetHandleMap = std::map <std::string, std::shared_ptr <AssetHandle>>;
	using AssetLoaders = std::list	<std::shared_ptr <IAssetLoader>>;
#pragma endregion

	class AssetCache
	{
		friend class AssetHandle;

		AssetHandleList	m_lru;					// the least recently used (LRU)
		AssetHandleMap	m_assetMap;				// asset descriptor map
		AssetLoaders	m_assetLoaders;

		IResourceProvider* m_resourceProvider;	// ResourceZipFile or ResourceDirectory

		unsigned int	m_cacheSize;
		unsigned int	m_allocated;

	public:
		AssetCache(const unsigned int sizeInMb, IResourceProvider *resourceProvider);
		virtual ~AssetCache();

		bool Init();

		void RegisterLoader(std::shared_ptr<IAssetLoader> assetLoader);

		std::shared_ptr<AssetHandle> GetHandle(Asset* asset);

		int Preload(const std::string pattern, void(*progressCallback)(int, bool &));
		std::vector<std::string> Match(const std::string pattern);

		void Flush();

		bool IsUsingDevelopmentDirectories() const;

	protected:
		bool MakeRoom(unsigned int size);
		char *Allocate(unsigned int size);
		void Free(std::shared_ptr<AssetHandle> gonner);

		std::shared_ptr<AssetHandle> Load(Asset* asset);
		std::shared_ptr<AssetHandle> Find(Asset* asset);

		void Update(std::shared_ptr<AssetHandle> assetHandle);
		void FreeOneResource();
		void MemoryHasBeenFreed(unsigned int size);

	};

}}