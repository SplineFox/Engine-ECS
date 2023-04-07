#include "Asset/AssetHandle.h"

namespace FlatFormer { namespace Asset {

	AssetHandle::AssetHandle(Asset & asset, char * buffer, unsigned int size, AssetCache* assetCache)
		: m_asset(asset)
	{
		m_buffer = buffer;
		m_size = size;
		m_payload = NULL;
		m_assetCache = assetCache;
	}

	AssetHandle::~AssetHandle()
	{
		delete m_buffer;
		m_assetCache->MemoryHasBeenFreed(m_size);
	}



}}