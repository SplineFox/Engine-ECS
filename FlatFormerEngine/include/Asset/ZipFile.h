#pragma once

#include "Engine/API.h"

#include "zlib.h"

namespace FlatFormer { namespace Asset {

	// Zip file consists of three areas::
	// - compressed/uncompressed data, (sequence of LocalFileHeader type structures, data itself and optional DataDescriptor)
	// - central directory (sequence of CentralDirectoryFileHeader structures)
	// - description of the central directory (End of central directory record (EOCD))
	//
	// At the beginning of the file, there is a set of LocalFileHeader, the data itself, and (optionally) the Data descriptor structure.
	// Then structures like CentralDirectoryFileHeader for each file and folder in the ZIP archive and completes all this with the
	// End of central directory record.
	//
	//		 ___________________________________
	//		|				Zip					|
	//		|___________________________________|
	//		|	File 0 :	- LocalFileHeader	|
	//		|				- File Data			|
	//		|	File 1							|
	//		|	File 2							|
	//		|	...								|
	//		|	File N							|
	//		|___________________________________|
	//		|	CentralDirectoryFileHeader[0]	|
	//		|	CentralDirectoryFileHeader[1]	|
	//		|	CentralDirectoryFileHeader[2]	|
	//		|	...								|
	//		|	CentralDirectoryFileHeader[N]	|
	//		|___________________________________|
	//		|	EOCD							|
	//		|___________________________________|
	//
	// Folders in a Zip file are represented by two structures LocalFileHeader and CentralDirectoryFileHeader
	// with zero size and checksum. The folder name ends with a slash "/"


	class ZipFile
	{
		struct LocalFileHeader;								// ZipLocalHeader
		struct CentralDirectoryFileHeader;					// ZipDirFileHeader
		struct EOCD;										// ZipDirHeader

	#pragma region Using
		// contains the correspondence of filenames and their positions inside the Zip
		using ZipContentMap = std::unordered_map<std::string, int>;
		using CentralDirEntries = std::vector<CentralDirectoryFileHeader*>;
	#pragma endregion

	private:
		int	m_numEntries;
		std::fstream m_file;

		ZipContentMap m_zipContentsMap;
		CentralDirEntries m_centralDirEntries;

	public:
		ZipFile();
		virtual ~ZipFile();

		bool Open(const std::wstring& fileName);
		
		void Close();

		int GetNumFiles() const;
		
		std::string GetFileName(int index);
		
		int GetFileLength(int index) const;
		
		int Find(const std::string& name) const;

		bool ReadFile(int index, void *buffer);
	};

}}