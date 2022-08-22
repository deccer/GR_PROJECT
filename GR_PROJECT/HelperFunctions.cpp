#include "HelperFunctions.hpp"

#include <snappy.h>

bool LoadBinaryData(const char* filePath, std::vector<uint8_t>* fileDataOut)
{
	std::fstream file;
	file.open(filePath, std::ios::binary);
	if (!file.is_open())
		return false;

	file.seekg(std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize <= 0)
		return false;

	file.seekg(std::ios::beg);

	fileDataOut->resize(fileSize);
	file.read((char*)fileDataOut->data(), fileSize);
	if (!file.good())
		return false;

	file.close();
	
	return true;
}

bool LoadCompressedBinaryData(const char* filePath, std::vector<uint8_t>* fileDataOut)
{
	std::vector<uint8_t> fileData;
	if (!LoadBinaryData(filePath, &fileData))
		return false;

	if (!snappy::IsValidCompressedBuffer((char*)fileData.data(), fileData.size()))
		return false;

	size_t uncompressedSize;
	if (!snappy::GetUncompressedLength((char*)fileData.data(), fileData.size(), &uncompressedSize))
		return false;

	fileDataOut->resize(uncompressedSize);

	if (!snappy::RawUncompress((char*)fileData.data(), fileData.size(), (char*)fileDataOut->data()))
		return false;
	
	return true;
}