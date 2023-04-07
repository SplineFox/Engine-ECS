#pragma once

#include "Engine/API.h"

#include "Asset/Asset.h"
#include "Asset/IResourceProvider.h"

namespace FlatFormer{ namespace Asset {

	class ResourceDirectory : public IResourceProvider
	{
#pragma region Using
		// contains matching of filenames and their positions
		using ContentMap = std::unordered_map<std::string, int>;
#pragma endregion

	public:
		enum Mode
		{
			Development, /// this mode checks the resource source directory for changes - helps during development
			Editor		 /// this mode checks resource directory - ZIP file remains unopened (MAIN)
		};

	private:
		Mode m_mode;
		std::string m_assetsDir;
		std::vector<WIN32_FIND_DATA> m_assetFileInfo;
		ContentMap m_directoryContentsMap;

	public:
		ResourceDirectory(const Mode mode = Development);

		bool Open()												override;
		int GetRawResourceSize(const Asset &asset)				override;
		int GetRawResource(const Asset &asset, char *buffer)	override;
		int GetNumResources() const								override;
		std::string GetResourceName(int index) const			override;
		bool IsUsingDevelopmentDirectories(void) const			override;

		int Find(const std::string& fileName);

	private:
		void ReadAssetsDirectory(std::string fileSpec);
	};

}}
