#pragma once

#include "Asset/AssetCache.h"

namespace FlatFormer {
	namespace Service {

		class AssetCacheUser
		{
		private:
			static Asset::AssetCache* s_AssetCacheService;

		protected:
			static Asset::AssetCache& GetAssetCacheService()
			{
				assert(s_AssetCacheService != nullptr && "ASSERT: s_AssetCacheService was nullptr!");
				return *s_AssetCacheService;
			}

		public:
			static void ProvideAssetCacheService(Asset::AssetCache* assetCacheInstance)
			{
				s_AssetCacheService = assetCacheInstance;
			}
		};


		using FFE_SERV_USER_AssetCache = FlatFormer::Service::AssetCacheUser;
	}
}