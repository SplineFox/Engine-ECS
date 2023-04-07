#include "Asset/ZipFile.h"

namespace FlatFormer { namespace Asset {

/// command to the VisualStudio compiler is that you do not need to use alignment inside structures
#pragma pack(1)

	struct ZipFile::LocalFileHeader
	{
		enum { SIGNATURE = 0x04034b50 };

		uint32 signature;				// 0x04034b50
		uint16 versionToExtract;
		uint16 generalPurposeBitFlag;	// has id 0x02
		uint16 compressionMethod;		// 0 - no compression, 0x08 - deflate
		uint16 modificationTime;
		uint16 modificationDate;
		uint32 crc32;
		uint32 compressedSize;
		uint32 uncompressedSize;
		uint16 filenameLength;
		uint16 extraFieldLength;

		//[followed at the end, but not part of the structure]:
		//------------------------------------------------ --
		// File name (size filenameLength)
		///uint8 *filename;
		// Additional data (sizeof "extraFieldLength")
		///uint8 *extraField;

		LocalFileHeader() :
			signature(0),
			versionToExtract(0),
			generalPurposeBitFlag(0),
			compressionMethod(0),
			modificationTime(0),
			modificationDate(0),
			crc32(0),
			compressedSize(0),
			uncompressedSize(0),
			filenameLength(0),
			extraFieldLength(0)
		{}
	};



	// Central Directory File Header
	//
	// Extended file metadata description.
	// Contains an enhanced version of LocalFileHeader
	// (fields, disk number, etc. are added)
	struct ZipFile::CentralDirectoryFileHeader
	{
		enum { SIGNATURE = 0x02014b50 };

		uint32 signature;			//0x02014b50 
		uint16 versionMadeBy;
		uint16 versionToExtract;
		uint16 generalPurposeBitFlag;
		uint16 compressionMethod;	//0 - no compression, 0x08 - deflate
		uint16 modificationTime;
		uint16 modificationDate;
		uint32 crc32;
		uint32 compressedSize;
		uint32 uncompressedSize;
		uint16 filenameLength;
		uint16 extraFieldLength;
		uint16 fileCommentLength;
		uint16 diskNumber;
		uint16 internalFileAttributes;
		uint32 externalFileAttributes;
		uint32 localFileHeaderOffset;

		//[followed at the end, but not part of the structure]:
		//------------------------------------------------ --
		// Filename (length filenameLength)
		///uint8 *filename;
		// Additional data (of length extraFieldLength)
		///uint8 *extraField;
		// File comment (length fileCommentLength)
		///uint8 *fileComment;

		CentralDirectoryFileHeader() :
			signature(0),
			versionMadeBy(0),
			versionToExtract(0),
			generalPurposeBitFlag(0),
			compressionMethod(0),
			modificationTime(0),
			modificationDate(0),
			crc32(0),
			compressedSize(0),
			uncompressedSize(0),
			filenameLength(0),
			extraFieldLength(0),
			fileCommentLength(0),
			diskNumber(0),
			internalFileAttributes(0),
			externalFileAttributes(0),
			localFileHeaderOffset(0)
		{}
	};



	// End Of Central Directory Record (EOCD)
	//
	// This structure is written at the end of the file.
	// Contains general information about the zip file.
	struct ZipFile::EOCD
	{
		enum { SIGNATURE = 0x06054b50 };

		uint32_t signature;						//0x06054b50
		uint16_t diskNumber;
		uint16_t startDiskNumber;				// Drive number where the beginning of the Central Directory is located
		uint16_t numberCentralDirectoryRecord;	// Number of entries in the Central Directory in the current disk
		uint16_t totalCentralDirectoryRecord;	//Total entries in the Central Directory
		uint32_t sizeOfCentralDirectory;
		uint32_t centralDirectoryOffset;
		uint16_t commentLength;

		//[followed at the end, but not part of the structure]:
		//------------------------------------------------ --
		// Comment (of length commentLength)
		///uint8_t *comment;

		EOCD() :
			signature(0),
			diskNumber(0),
			startDiskNumber(0),
			numberCentralDirectoryRecord(0),
			totalCentralDirectoryRecord(0),
			sizeOfCentralDirectory(0),
			centralDirectoryOffset(0),
			commentLength(0)
		{}
	};
#pragma pack()

	ZipFile::ZipFile() :
		m_numEntries(0)
	{}


	ZipFile::~ZipFile()
	{
		Close();
	}

	
	bool ZipFile::Open(const std::wstring & fileName)
	{
		m_file.open(fileName, std::ios::in | std::ios::binary);
		if (!m_file.is_open())
			return false;

		EOCD eocd;

		/// go to the end of the file
		m_file.seekg(0, m_file.end);
		/// get how many read
		size_t fileSize = m_file.tellg();

		/// read EOCD
		size_t offset = fileSize - sizeof(eocd);
		m_file.seekg(offset, m_file.beg);
		m_file.read((char*)&eocd, sizeof(eocd));

		/// check EOCD signature
		if (eocd.signature != EOCD::SIGNATURE)
		{
			return false;
		}
		/// ---------------------------------------------
		
		// load the Central Directory File Header entries
		/// move to the offset of the first record
		m_file.seekg(eocd.centralDirectoryOffset, m_file.beg);
		/// bypass all records of the Central Directory block
		for (uint16 index = 0; index < eocd.numberCentralDirectoryRecord; ++index)
		{
			CentralDirectoryFileHeader* dirEntry = new CentralDirectoryFileHeader();
			m_centralDirEntries.emplace_back(dirEntry);

			/// read record
			m_file.read((char*)dirEntry, sizeof(*dirEntry));
			/// check the signature for correctness
			if (dirEntry->signature != CentralDirectoryFileHeader::SIGNATURE)
				return false;

			/// read file/folder name
			if (dirEntry->filenameLength)
			{
				/// filenameLength buffer + line terminator
				char* filename = new char[dirEntry->filenameLength + 1];
				m_file.read((char*)filename, dirEntry->filenameLength);
				/// add line ending character
				filename[dirEntry->filenameLength] = 0;
				/// convert buffer to string
				std::string name = filename;
				/// convert to lowercase
				std::transform(name.begin(), name.end(), name.begin(), tolower);
				/// add to the map
				m_zipContentsMap[name] = index;
				/// free buffer
				delete[] filename;
			}

			/// skip additional fields
			m_file.seekg(dirEntry->extraFieldLength, m_file.cur);
			/// skip comment
			m_file.seekg(dirEntry->fileCommentLength, m_file.cur);
		}

		m_numEntries = eocd.numberCentralDirectoryRecord;
		return true;
	}


	void ZipFile::Close()
	{
		m_file.close();
		m_zipContentsMap.clear();
		m_numEntries = 0;
		/// deleting Central Directory entries
		for (CentralDirEntries::iterator iterator = this->m_centralDirEntries.begin(); iterator != this->m_centralDirEntries.end(); ++iterator)
		{
			delete *iterator;
			*iterator = nullptr;
		}
	}


	// Returns the number of files
	int ZipFile::GetNumFiles() const
	{
		return m_numEntries;
	}


	// Getting filename by index
	std::string ZipFile::GetFileName(int index)
	{
		std::string fileName = "";
		return fileName;
	}


	// Getting the uncompressed length of a file
	int ZipFile::GetFileLength(int index) const
	{
		if (index < 0 || index >= m_numEntries)
			return -1;
		
		return m_centralDirEntries[index]->uncompressedSize;
	}


	// Getting the index of a file by name
	int ZipFile::Find(const std::string& name) const
	{
		std::string lowerCase = name;
		std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), tolower);

		ZipContentMap::const_iterator iterator = m_zipContentsMap.find(lowerCase);
		if (iterator == m_zipContentsMap.end())
			return -1;

		return iterator->second;
	}


	// Reading a file by index into a buffer
	bool ZipFile::ReadFile(int index, void * buffer)
	{
		if (buffer == nullptr || index < 0 || index > m_numEntries)
			return false;
		
		/// go to the Local File Header of the file at index
		m_file.seekg(m_centralDirEntries[index]->localFileHeaderOffset, m_file.beg);
		LocalFileHeader fileHeader;
		/// read file
		m_file.read((char*)&fileHeader, sizeof(fileHeader));

		if (fileHeader.signature != LocalFileHeader::SIGNATURE)
			return false;

		/// skip file name
		m_file.seekg(fileHeader.filenameLength, m_file.cur);
		/// skip extra
		m_file.seekg(fileHeader.extraFieldLength, m_file.cur);
		/// if compression is not applied, then just read
		if (fileHeader.compressionMethod == Z_NO_COMPRESSION)
		{
			m_file.read((char*)buffer, fileHeader.compressedSize);
			return true;
		}
		else if (fileHeader.compressionMethod != Z_DEFLATED)
			return false;

		/// create a buffer for compressed data
		char* compressedData = new char[fileHeader.compressedSize];
		if (!compressedData)
			return false;

		/// initialize the buffer with zeros
		memset(compressedData, 0, fileHeader.compressedSize);
		/// read compressed data
		m_file.read(compressedData, fileHeader.compressedSize);

		bool success = true;
		/// initialize structure for unpacking
		z_stream zStream;
		memset(&zStream, 0, sizeof(zStream));
		/// input buffer size
		zStream.avail_in = (uInt)fileHeader.compressedSize;
		/// input buffer
		zStream.next_in = (Bytef*)compressedData;
		/// output buffer size
		zStream.avail_out = fileHeader.uncompressedSize;
		/// output buffer
		zStream.next_out = (Bytef*)buffer;

		auto error = inflateInit2(&zStream, -MAX_WBITS);
		if (error == Z_OK)
		{
			/// perform unpacking
			inflate(&zStream, Z_FINISH);
			/// clear structure for unpacking
			inflateEnd(&zStream);
		}
		if (error != Z_OK)
			success = false;

		delete[] compressedData;
		return success;
	}

}}