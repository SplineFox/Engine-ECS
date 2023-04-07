#pragma once

#include "Asset/Asset.h"
#include "Asset/IResourceProvider.h"
#include "Asset/ZipFile.h"

namespace FlatFormer{ namespace Asset {

	class ResourceZipFile : public IResourceProvider
	{
	private:
		ZipFile *m_zipFile;
		std::wstring m_fileName;

	public:
		ResourceZipFile(const std::wstring resFileName);
		~ResourceZipFile();

		bool Open()												override;
		int GetRawResourceSize(const Asset &asset)				override;
		int GetRawResource(const Asset &asset, char *buffer)	override;
		int GetNumResources() const								override;
		std::string GetResourceName(int index) const			override;
		bool IsUsingDevelopmentDirectories(void) const			override;
	};

}}