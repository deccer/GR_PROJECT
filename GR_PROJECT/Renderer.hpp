#pragma once

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <map>
#include <list>
#include <vector>
#include <cstdint>
#include <iostream>

typedef uint32_t GRModelHandle;
typedef uint32_t GRTextureHandle;
typedef uint32_t GRPipelineHandle;
typedef uint32_t GRMaterialHandle;
typedef uint32_t GRRenderObjectHandle;

typedef enum class GR_MODEL_TYPE {
	GR_STATIC_MESH = 0,
	GR_SKINNED_MESH = 1
};

typedef enum class GR_PIPELINE_TYPE {
	GR_DEFERRED_PIPELINE = 0,
	GR_FORWARD_OPAQUE = 1,
	GR_FORWARD_TRANSPARENT = 2,
};

struct GRPipelineCreateInfo {
	GR_PIPELINE_TYPE pipelineType;
	size_t shaderCodeByteSize;
	void* pShaderCodeData;
};

struct GRMaterialCreateInfo {
	uint8_t numTextures;
	GRPipelineHandle pipeline;
	GRModelHandle model;
	GRTextureHandle textures[8];
};

class Renderer
{
protected:
	GLFWwindow* _window = nullptr;

	std::map<const char*, GRModelHandle> _modelNames;
	std::list<GRModelHandle> _freedModelHandles;

	std::map<const char*, GRTextureHandle> _textureNames;
	std::list<GRTextureHandle> _freedTextureHandles;

	std::map<const char*, GRPipelineHandle> _pipelineNames;
	std::list<GRPipelineHandle> _freedPipelineHandles;

	std::map<const char*, GRMaterialHandle> _materialNames;
	std::list<GRMaterialHandle> _freedMaterialHandles;

	std::list<GRRenderObjectHandle> _freedRenderObjectHandles;

	bool LoadModelData(const char* modelPath, const char* modelName, GR_MODEL_TYPE modelType, std::vector<uint8_t>* modelDataOut);

	Renderer(GLFWwindow* window)
		: _window(window) {}

public:
	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	/*virtual GRModelHandle CreateModel(const char* modelPath, const char* modelName, GR_MODEL_TYPE modelType);
	virtual bool DestroyModel(const GRModelHandle modelHandle) = 0;

	virtual GRTextureHandle CreateTexture() = 0;
	virtual bool DestroyTexture(const GRTextureHandle textureHandle) = 0;

	virtual GRPipelineHandle CreatePipeline(const GRPipelineCreateInfo& pipelineCreateInfo) = 0;
	virtual bool DestroyPipeline(const GRPipelineHandle pipelineHandle) = 0;

	virtual GRMaterialHandle CreateMaterial() = 0;
	virtual bool DestroyMaterial(const GRMaterialHandle materialHandle) = 0;

	virtual GRRenderObjectHandle CreateRenderObject() = 0;
	virtual bool DestroyRenderObject(const GRRenderObjectHandle renderObjectHandle) = 0;

	virtual void SubmitRenderObjects(const GRRenderObjectHandle* renderObjectHandles, const uint32_t numRenderObjectHandles) = 0;*/
	virtual void Draw() = 0;
};