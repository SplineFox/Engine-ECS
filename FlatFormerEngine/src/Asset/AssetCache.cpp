#include "Asset/AssetCache.h"

#include "Asset/AssetHandle.h"

namespace FlatFormer { namespace Asset{

	// Method for comparing strings with a pattern
	bool WildcardMatch(const char *pattern, const char *string)
	{
		int i, star;
	new_segment:

		star = 0;
		if (*pattern == '*') {
			star = 1;
			do { pattern++; } while (*pattern == '*');
		}

	test_match:

		for (i = 0; pattern[i] && (pattern[i] != '*'); i++) {
			if (string[i] != pattern[i]) {
				if (!string[i]) return 0;
				if ((pattern[i] == '?') && (string[i] != '.')) continue;
				if (!star) return 0;
				string++;
				goto test_match;
			}
		}
		if (pattern[i] == '*') {
			string += i;
			pattern += i;
			goto new_segment;
		}
		if (!string[i]) return 1;
		if (i && pattern[i - 1] == '*') return 1;
		if (!star) return 0;
		string++;
		goto test_match;
	}



	AssetCache::AssetCache(const unsigned int sizeInMb, IResourceProvider * resourceProvider)
	{
		m_cacheSize = sizeInMb * 1024 * 1024;
		m_allocated = 0;
		m_resourceProvider = resourceProvider;
	}


	AssetCache::~AssetCache()
	{
		while (!m_lru.empty())
		{
			FreeOneResource();
		}
		if (m_resourceProvider)
		{
			delete m_resourceProvider;
			m_resourceProvider = nullptr;
		}
	}


	bool AssetCache::Init()
	{
		bool success = false;
		if (m_resourceProvider->Open())
		{
			RegisterLoader(std::shared_ptr<IAssetLoader>(new DefaultAssetLoader()));
			RegisterLoader(std::shared_ptr<IAssetLoader>(new PNGAssetLoader()));
			RegisterLoader(std::shared_ptr<IAssetLoader>(new JPGAssetLoader()));
			RegisterLoader(std::shared_ptr<IAssetLoader>(new TTFAssetLoader()));

			success = true;
		}
		return success;
	}


	void AssetCache::RegisterLoader(std::shared_ptr<IAssetLoader> assetLoader)
	{
		m_assetLoaders.push_front(assetLoader);
	}


	std::shared_ptr<AssetHandle> AssetCache::GetHandle(Asset* asset)
	{
		std::shared_ptr<AssetHandle> assethandle(Find(asset));
		if (assethandle == nullptr)
		{
			assethandle = Load(asset);
			assert(assethandle && "ASSERT: [GetGandle] assetHandle was nullptr!");
		}
		else
		{
			Update(assethandle);
		}
		return assethandle;
	}


	int AssetCache::Preload(const std::string pattern, void(*progressCallback)(int, bool &))
	{
		if (m_resourceProvider == nullptr)
			return 0;

		int numFiles = m_resourceProvider->GetNumResources();
		int loaded = 0;
		bool cancel = false;
		for (int index = 0; index < numFiles; ++index)
		{
			Asset asset(m_resourceProvider->GetResourceName(index));

			if (WildcardMatch(pattern.c_str(), asset.m_name.c_str()))
			{
				std::shared_ptr<AssetHandle> handle = GetHandle(&asset);
				++loaded;
			}

			if (progressCallback != NULL)
			{
				progressCallback(index * 100 / numFiles, cancel);
			}
		}
		return loaded;
	}


	std::vector<std::string> AssetCache::Match(const std::string pattern)
	{
		std::vector<std::string> matchingNames;
		if (m_resourceProvider == nullptr)
			return matchingNames;

		int numFiles = m_resourceProvider->GetNumResources();
		for (int index = 0; index < numFiles; ++index)
		{
			std::string name = m_resourceProvider->GetResourceName(index);
			std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
			if (WildcardMatch(pattern.c_str(), name.c_str()))
			{
				matchingNames.push_back(name);
			}
		}
		return matchingNames;
	}


	// Frees all handles in the cache.
	// This is useful when loading a new level or if you need to update all the data.
	void AssetCache::Flush()
	{
		while (!m_lru.empty())
		{
			std::shared_ptr<AssetHandle> assetHandle = *(m_lru.begin());
			Free(assetHandle);
			m_lru.pop_front();
		}
	}


	// Lets you know if the resource provider is using a directory or a packaged file
	bool AssetCache::IsUsingDevelopmentDirectories() const
	{
		assert(m_resourceProvider);
		return m_resourceProvider->IsUsingDevelopmentDirectories();
	}





	bool AssetCache::MakeRoom(unsigned int size)
	{
		if (size > m_cacheSize)
		{
			return false;
		}

		/// ���������� null ���� ��� ����������� �������� ������
		while (size > (m_cacheSize - m_allocated))
		{
			/// ��� ���� � ������� �� ��� ������������
			if (m_lru.empty())
				return false;
			/// ����������� ���� ������
			FreeOneResource();
		}

		return true;
	}


	char* AssetCache::Allocate(unsigned int size)
	{
		if (!MakeRoom(size))
			return nullptr;

		char *mem = new char[size];
		if (mem)
		{
			m_allocated += size;
		}

		return mem;
	}


	void AssetCache::Free(std::shared_ptr<AssetHandle> gonner)
	{
		m_lru.remove(gonner);
		m_assetMap.erase(gonner->m_asset.m_name);
		/// Note - the resource might still be in use by something,
		/// so the cache can't actually count the memory freed until the
		/// ResHandle pointing to it is destroyed.
		
		///m_allocated -= gonner->m_resource.m_size;
		///delete gonner;
	}


	// �������� ������ ������� � ���������� ��� � LRU � Map
	std::shared_ptr<AssetHandle> AssetCache::Load(Asset* asset)
	{
		std::shared_ptr<IAssetLoader> loader;
		std::shared_ptr<AssetHandle> handle;

		/// �������� ��� ����������
		for (AssetLoaders::iterator it = m_assetLoaders.begin(); it != m_assetLoaders.end(); ++it)
		{
			std::shared_ptr<IAssetLoader> testLoader = *it;
			/// ���� ������� ��������, �������� ������� ����������
			if (WildcardMatch(testLoader->GetPattern().c_str(), asset->m_name.c_str()))
			{
				loader = testLoader;
				break;
			}
		}

		/// ���� �� ���� ��������� �� ������
		if (!loader)
		{
			assert(loader && "Default resource loader not found!");
			return handle;
		}

		/// �������� ������ ������� �� Zip �����
		int rawSize = m_resourceProvider->GetRawResourceSize(*asset);
		if (rawSize < 0)
		{
			assert(rawSize > 0 && "Resource not found");
			return handle;
		}

		/// ���� ������ �� ������� ���������, �� ������ ���������� ����� Allocate
		/// ���� - �������� ��������� �����
		char *rawBuffer = loader->IsUseRawFile() ? Allocate(rawSize) : new char[rawSize];
		memset(rawBuffer, 0, rawSize);

		/// ���� ������ �������� �������, ������ ����������� ������� GetRawResource,
		/// ����� - ������������ �������������������� AssetHandle
		if (rawBuffer == nullptr || m_resourceProvider->GetRawResource(*asset, rawBuffer) == 0)
		{
			/// ���� �� ������� ������
			return handle;
		}

		/// ��������� ������� ������� ���������, � ���������� �������, �������� ������
		/// ����� ���������� �� ���������, ������� ��������� ��� ���� ������
		char *buffer = nullptr;
		unsigned int size = 0;

		/// ���� ������ �� ������� ���������, �� ������ ���������� ��� ����������
		if (loader->IsUseRawFile())
		{
			buffer = rawBuffer;
			handle = std::shared_ptr<AssetHandle>(new AssetHandle(*asset, buffer, rawSize, this));
		}
		else
		{
			/// �������� �������� ������ ������� ����� ��������
			size = loader->GetLoadedResourceSize(rawBuffer, rawSize);
			/// �������� ������ ����
			buffer = Allocate(size);
			if (rawBuffer == nullptr || buffer == nullptr)
			{
				/// ���� �� ������� ������
				return std::shared_ptr<AssetHandle>();
			}
			/// ������ ���������� ��� ������������ �������
			handle = std::shared_ptr<AssetHandle>(new AssetHandle(*asset, buffer, size, this));
			/// ��������� ���������� �����������
			bool success = loader->LoadResource(rawBuffer, rawSize, handle);

			if (loader->IsDiscardRawBufferAfterLoad())
			{
				if(rawBuffer)
					delete[] rawBuffer;
			}

			if (!success)
			{
				/// ���� �� ������� ������
				return std::shared_ptr<AssetHandle>();
			}
		}

		if (handle)
		{
			m_lru.push_front(handle);
			m_assetMap[asset->m_name] = handle;
		}

		assert(loader && "Default resource loader not found!");
		/// ���� �� ������� ������
		return handle;
	}


	std::shared_ptr<AssetHandle> AssetCache::Find(Asset* asset)
	{
		AssetHandleMap::iterator index = m_assetMap.find(asset->m_name);
		if (index == m_assetMap.end())
			return std::shared_ptr<AssetHandle>();

		return index->second;
	}


	/// ��������� ������� ����������� � ������ LRU
	void AssetCache::Update(std::shared_ptr<AssetHandle> assetHandle)
	{
		m_lru.remove(assetHandle);
		m_lru.push_front(assetHandle);
	}


	void AssetCache::FreeOneResource()
	{
		/// �������� ��������� ������ � LRU (�������� ��������������)
		AssetHandleList::iterator gonner = m_lru.end();
		gonner--;

		std::shared_ptr<AssetHandle> assetHandle = *gonner;

		m_lru.pop_back();
		m_assetMap.erase(assetHandle->m_asset.m_name);
		//NOTE: ���� ��� ������ ������ ������ ���� - ���� ������� �� ��� ����� ��������������
		// �����-���� �����������, �������� ���������� AssetHandle. ������ �����, ����� ���������
		// ��� ����������, ������ ������ ���� ������������� �����������.
	}


	void AssetCache::MemoryHasBeenFreed(unsigned int size)
	{
		m_allocated -= size;
	}

}}