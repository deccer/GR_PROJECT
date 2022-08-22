#include "Renderer.hpp"
#include "HelperFunctions.hpp"

bool Renderer::LoadModelData(const char* modelPath, const char* modelName, GR_MODEL_TYPE modelType, std::vector<uint8_t>* modelDataOut)
{
	std::vector<uint8_t> modelFileData;
	if (!LoadCompressedBinaryData(modelPath, &modelFileData))
		return false;

	return true;
}
