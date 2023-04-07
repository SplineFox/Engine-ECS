#include "Asset/ResourceZipFile.h"

namespace FlatFormer { namespace Asset {

	ResourceZipFile::ResourceZipFile(const std::wstring resFileName) :
		m_zipFile(nullptr),
		m_fileName(resFileName)
	{}


	ResourceZipFile::~ResourceZipFile()
	{
		if(m_zipFile) 
		{ 
			delete m_zipFile;
			m_zipFile = nullptr;
		}
	}


	bool ResourceZipFile::Open()
	{
		m_zipFile = new ZipFile();
		if (m_zipFile)
		{
			return m_zipFile->Open(m_fileName.c_str());
		}
		return false;
	}


	int ResourceZipFile::GetRawResourceSize(const Asset & asset)
	{
		int resourceIndex = m_zipFile->Find(asset.m_name.c_str());
		if (resourceIndex == -1)
			return -1;

		return m_zipFile->GetFileLength(resourceIndex);
	}


	int ResourceZipFile::GetRawResource(const Asset & asset, char * buffer)
	{
		int size = 0;
		int resourceIndex = m_zipFile->Find(asset.m_name.c_str());
		if (resourceIndex >= 0)
		{
			size = m_zipFile->GetFileLength(resourceIndex);
			m_zipFile->ReadFile(resourceIndex, buffer);
		}
		return size;
	}


	int ResourceZipFile::GetNumResources() const
	{
		return (m_zipFile == nullptr)? 0 : m_zipFile->GetNumFiles();
	}


	std::string ResourceZipFile::GetResourceName(int index) const
	{
		std::string resName = "";
		if (m_zipFile != NULL && index >= 0 && index < m_zipFile->GetNumFiles())
		{
			resName = m_zipFile->GetFileName(index);
		}
		return resName;
	}


	bool ResourceZipFile::IsUsingDevelopmentDirectories(void) const
	{
		return false;
	}

}}