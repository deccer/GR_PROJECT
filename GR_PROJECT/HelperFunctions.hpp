#pragma once

#include <vector>
#include <fstream>
#include <cstdint>

bool LoadBinaryData(const char* filePath, std::vector<uint8_t>* fileDataOut);
bool LoadCompressedBinaryData(const char* filePath, std::vector<uint8_t>* fileDataOut);