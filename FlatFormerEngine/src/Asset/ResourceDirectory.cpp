#include "Asset/ResourceDirectory.h"

namespace FlatFormer { namespace Asset {


	std::string WS2S(const std::wstring& s)
	{
		int slength = (int)s.length() + 1;
		int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0) - 1;
		std::string r(len, '\0');
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
		return r;
	}



	ResourceDirectory::ResourceDirectory(const Mode mode) :
		m_mode(mode)
	{
		TCHAR currentDirectory[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, currentDirectory);
		
		m_assetsDir = currentDirectory;						/// D:\SomeDir\AnotherDir\*.*
		int lastSlash = m_assetsDir.find_last_of("\\");		///	lastSlash  = index	 ^
		m_assetsDir = m_assetsDir.substr(0, lastSlash);		/// D:\SomeDir\AnotherDir
		m_assetsDir += "\\Assets\\";						/// D:\SomeDir\AnotherDir\Assets\*.*
	}


	bool ResourceDirectory::Open()
	{
		ReadAssetsDirectory("*");

		return true;
	}


	int ResourceDirectory::GetRawResourceSize(const Asset& asset)
	{
		int index = Find(asset.m_name.c_str());
		if (index == -1)
			return -1;

		return m_assetFileInfo[index].nFileSizeLow;
	}


	int ResourceDirectory::GetRawResource(const Asset& asset, char* buffer)
	{
		int size = 0;
		/// get asset index by its name
		int index = Find(asset.m_name.c_str());
		if (index >= 0)
		{
			std::string fullFileSpec = m_assetsDir + asset.m_name.c_str();
			std::ifstream file;
			file.open(fullFileSpec.c_str(), std::ios::in | std::ios::binary);
			file.read(buffer, m_assetFileInfo[index].nFileSizeLow);
			file.close();
			size = m_assetFileInfo[index].nFileSizeLow;
		}
		return size;
	}


	int ResourceDirectory::GetNumResources() const
	{
		return m_assetFileInfo.size();
	}


	std::string ResourceDirectory::GetResourceName(int index) const
	{
		return m_assetFileInfo[index].cFileName;
	}


	bool ResourceDirectory::IsUsingDevelopmentDirectories(void) const
	{
		return true;
	}


	int ResourceDirectory::Find(const std::string & fileName)
	{
		std::string lowerCase = fileName;
		std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
		ContentMap::const_iterator i = m_directoryContentsMap.find(lowerCase);
		if (i == m_directoryContentsMap.end())
			return -1;

		return i->second;
	}


	void ResourceDirectory::ReadAssetsDirectory(std::string fileSpec)
	{
		HANDLE fileHandle;
		WIN32_FIND_DATA findData;

		/// get first file
		std::string pathSpec = m_assetsDir + fileSpec;	/// D:\SomeDir\AnotherDir\Assets\*.*
		/// The FindFirstFile function looks for a file directory or subdirectory
		/// whose name matches the specified filename
		fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
		if (fileHandle != INVALID_HANDLE_VALUE)
		{
			while (FindNextFile(fileHandle, &findData))
			{
				/// if it is a hidden file
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					continue;

				std::string fileName = findData.cFileName;

				/// if it is a directory
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (fileName != ".." && fileName != ".")
					{
						fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName + "\\*";
						/// recursively traverse the directory
						ReadAssetsDirectory(fileName);
					}
				}
				/// if it is a file
				else
				{
					fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName;
					std::string lowerCase = fileName;
					std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
					strcpy_s(&findData.cFileName[0], MAX_PATH, lowerCase.c_str());
					m_directoryContentsMap[lowerCase] = m_assetFileInfo.size();
					m_assetFileInfo.push_back(findData);
				}
			}
		}
		FindClose(fileHandle);
	}

}}