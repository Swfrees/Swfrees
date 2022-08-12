#pragma once
#include "../Gateware/Gateware.h"
#include <vulkan/vulkan_core.h>
#include "h2bParser.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define MAX_SUBMESH_PER_DRAW 1024

enum ObjectType
{
	NONE = 0,
	MODEL,
	LIGHT
};

enum LightType
{
	PLIGHT,
	DLIGHT
};

struct Model
{
	std::string ModelName;
	
	VkBuffer ModelVertexBuffer;
	VkBuffer ModelIndexBuffer;
	VkDeviceMemory ModelVertexMemory;
	VkDeviceMemory ModelIndexMemory;

	std::vector<H2B::VERTEX> ModelVertices;
	std::vector<unsigned> ModelIndices;
	std::vector<H2B::BATCH> ModelSubmeshes;
	std::vector<H2B::MATERIAL> ModelMaterials;

	GW::MATH::GMATRIXF ModelWorldMatrix;

	std::vector<int> SubmeshMaterials;

	VkBuffer BoundingBoxesVertexBuffer;
	VkBuffer BoundingBoxesIndexBuffer;
	VkDeviceMemory BoundingBoxesVertexMemory;
	VkDeviceMemory BoundingBoxesIndexMemory;

	std::vector<H2B::VERTEX> BoundingBoxesVertices;
	GW::MATH::GVECTORF BoundingBoxesBounds;
};

struct Light
{
	LightType LightType;
	GW::MATH::GVECTORF LightPosition;
	GW::MATH::GVECTORF LightDirection;
	GW::MATH::GVECTORF LightColor;
	float Falloff;
};

struct LevelObject
{
	ObjectType ObjectType;
	std::string ObjectName;
	GW::MATH::GMATRIXF ObjectWorldMatrix;

	std::vector<H2B::VERTEX> BoundingBoxesVertices;
	GW::MATH::GVECTORF BoundingBoxesBounds;
};

struct Level
{
	std::string LevelPath;
	std::vector <LevelObject> LevelObjects;
	std::vector<Model> LevelModels;
	std::vector<Light> LevelLights;
	bool BoundingBoxFlag;
	bool TriangulateFlag;
};

struct SHADER_MODEL_DATA
{
	//globally shared model data
	//GW::MATH::GVECTORF sundirection, sunColor; //lighting info
	int DirectionalLightCount, PointLightCount;

	int padding[2];

	GW::MATH::GVECTORF DirectionalLightDirection[MAX_SUBMESH_PER_DRAW];
	GW::MATH::GVECTORF DirectionalLightColor[MAX_SUBMESH_PER_DRAW];

	GW::MATH::GVECTORF PointLightPosition[MAX_SUBMESH_PER_DRAW];
	GW::MATH::GVECTORF PointLightColor[MAX_SUBMESH_PER_DRAW];
	float PointLightFalloffs[MAX_SUBMESH_PER_DRAW];

	GW::MATH::GVECTORF camEye, sunAmbient;
	GW::MATH::GMATRIXF viewMatrix, projectionMatrix; //viewing info
	//per sub-mesh transform and material data
	GW::MATH::GMATRIXF matricies[MAX_SUBMESH_PER_DRAW];
	H2B::ATTRIBUTES materials[MAX_SUBMESH_PER_DRAW];
};

void GenerateBoundingBoxes(Level& InputLevel, GW::MATH::GVECTORF Bounds);
Level ParseLevel(std::string LevelName);
void LoadLevel(Level& InputLevel, VkDevice& device, VkPhysicalDevice& physicalDevice);
void SetupModelData(Level &InputLevel, SHADER_MODEL_DATA &modelData);
void RenderModels(Level InputLevel, std::vector<VkDescriptorSet> descriptorSet, VkCommandBuffer& commandBuffer, VkPipelineLayout& pipelineLayout, VkDeviceSize* offsets);
void LevelCleanup(Level& InputLevel, VkDevice& device);