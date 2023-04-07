#pragma once

#include "Engine/API.h"

#include "Asset/Asset.h"
#include "Asset/AssetCache.h"

namespace FlatFormer { namespace Asset {

#pragma region MyRegion
	using SharedAssetHandle = std::shared_ptr<AssetHandle>;
#pragma endregion

	// Asset descriptor
	// Serves to keep track of the loaded asset
	class AssetHandle
	{
		friend class AssetCache;

	protected:
		Asset m_asset;
		char *m_buffer;
		unsigned int m_size;
		std::shared_ptr<IAssetPayloadData> m_payload;
		AssetCache* m_assetCache;

	public:
		AssetHandle(Asset& asset, char *buffer, unsigned int size, AssetCache* assetCache);
		virtual ~AssetHandle();

		const std::string GetName()	{ return m_asset.m_name; }
		unsigned int Size() const	{ return m_size; }
		char* Buffer() const		{ return m_buffer; }
		char* WritableBuffer()		{ return m_buffer; }

		std::shared_ptr<IAssetPayloadData> GetPayload()				{ return m_payload; }
		void SetPayload(std::shared_ptr<IAssetPayloadData> payload) { m_payload = payload; }
	};

	// When the cache loads an asset, it creates an AssetHandle, allocates memory of the required size,
	// and reads the resource from the resource file.
	// 
	// AssetHandle exists in memory as long as AssetCache stores it
	// or as long as the user of the asset stores a shared_ptr on AssetHandle.
	//
	// AssetHandle also keeps track of the size of the memory block.
	// If AssetChache fills up, AssetHandle is released from the cache.
	//
	// The AssetHandle destructor makes a call to the MemoryHasBeenFreed method on AssetCache.
}}