// minimalistic code to draw a single triangle, this is not part of the API.
// TODO: Part 1b
#include "FSLogo.h"
#include "shaderc/shaderc.h" // needed for compiling shaders at runtime
#ifdef _WIN32 // must use MT platform DLL libraries on windows
	#pragma comment(lib, "shaderc_combined.lib") 
#endif
#include "XTime.h"
#include "Parser.h"

const double pi = 3.14159265358979323846;

//#define MAX_SUBMESH_PER_DRAW 1024
//struct SHADER_MODEL_DATA
//{
//	//globally shared model data
//	GW::MATH::GVECTORF sundirection, sunColor; //lighting info
//	GW::MATH::GVECTORF camEye, sunAmbient;
//	GW::MATH::GMATRIXF viewMatrix, projectionMatrix; //viewing info
//	//per sub-mesh transform and material data
//	GW::MATH::GMATRIXF matricies[MAX_SUBMESH_PER_DRAW];
//	OBJ_ATTRIBUTES materials[MAX_SUBMESH_PER_DRAW];
//};

std::string ShaderAsString(const char* shaderFilePath) {
	std::string output;
	unsigned int stringLength = 0;
	GW::SYSTEM::GFile file; file.Create();
	file.GetFileSize(shaderFilePath, stringLength);
	if (stringLength && +file.OpenBinaryRead(shaderFilePath)) {
		output.resize(stringLength);
		file.Read(&output[0], stringLength);
	}
	else
		std::cout << "ERROR: Shader Source File \"" << shaderFilePath << "\" Not Found!" << std::endl;
	return output;
}

extern VkClearValue clrAndDepth[2];

// Creation, Rendering & Cleanup
class Renderer
{

	Level LevelObject = {};

	// TODO: Part 2b
	
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GVulkanSurface vlk;
	GW::CORE::GEventReceiver shutdown;
	
	// what we need at a minimum to draw a triangle
	VkDevice device = nullptr;
	VkBuffer vertexHandle = nullptr;
	VkDeviceMemory vertexData = nullptr;
	VkBuffer indexHandle = nullptr;
	VkDeviceMemory indexData = nullptr;
	std::vector<VkBuffer> storageHandle;
	std::vector<VkDeviceMemory> storageData;
	// TODO: Part 1g
	// TODO: Part 2c
	VkShaderModule vertexShader = nullptr;
	VkShaderModule pixelShader = nullptr;
	// pipeline settings for drawing (also required)
	VkPipeline pipeline = nullptr;
	VkPipelineLayout pipelineLayout = nullptr;

	VkPipeline BoundingBoxesPipeline = nullptr;
	VkPipelineLayout BoundingBoxesPipelineLayout = nullptr;

	VkDescriptorSetLayout descriptorLayout = nullptr;
	VkDescriptorPool descriptorPool = nullptr;

	std::vector<VkDescriptorSet> descriptorSet;
	// TODO: Part 2e
	// TODO: Part 2f
	// TODO: Part 2g
		// TODO: Part 4f
		
	// TODO: Part 2a
	GW::MATH::GMatrix mathOperator = GW::MATH::GMatrix();
	GW::MATH::GMatrix ProxyMatrix = GW::MATH::GMatrix();
	GW::INPUT::GInput ProxyInput = GW::INPUT::GInput();
	GW::INPUT::GController ProxyController = GW::INPUT::GController();
	GW::MATH::GMATRIXF worldMatrix = GW::MATH::GIdentityMatrixF;
	GW::MATH::GMATRIXF rotationWorldMatrix = GW::MATH::GIdentityMatrixF;
	GW::MATH::GMATRIXF viewMatrix = GW::MATH::GIdentityMatrixF;
	GW::MATH::GVECTORF Eye = GW::MATH::GVECTORF();// (0.75, 0.25, -1.5)
	GW::MATH::GVECTORF Look = GW::MATH::GVECTORF();
	GW::MATH::GVECTORF Up = GW::MATH::GVECTORF();
	GW::MATH::GMATRIXF projectionMatrix = GW::MATH::GIdentityMatrixF;
	GW::MATH::GVECTORF LightDirection = GW::MATH::GVECTORF();
	GW::MATH::GVECTORF lightColor = GW::MATH::GVECTORF();
	GW::MATH::GVECTORF camPos = GW::MATH::GVECTORF();
	GW::MATH::GVECTORF sunAmbient = GW::MATH::GVECTORF();

	SHADER_MODEL_DATA modelData = SHADER_MODEL_DATA();

	std::vector<std::string> LevelVector = std::vector<std::string>();
	int LevelNumber = 0;
	// TODO: Part 2b
	// TODO: Part 4g
	
public:
	float aspect;
	VkPhysicalDevice physicalDevice = nullptr;
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GVulkanSurface _vlk)
	{
		win = _win;
		vlk = _vlk;
		unsigned int width, height;
		win.GetClientWidth(width);
		win.GetClientHeight(height);
		vlk.GetAspectRatio(aspect);
		// TODO: Part 2a

		ProxyMatrix.Create();
		ProxyInput.Create(win);
		ProxyController.Create();

		//mathOperator.RotateYGlobalF(worldMatrix, 3.214f, worldMatrix);
		Eye.x = 0.0f;
		Eye.y = 0.00f;
		Eye.z = 0.50f;
		Look.x = 0.0f;
		Look.y = 5.0f;
		Look.z = -1.50f;
		Up.x = 0.0f;
		Up.y = 1.0f;
		Up.z = 0.0f;
		mathOperator.LookAtLHF(Eye, Look, Up, viewMatrix);
		mathOperator.ProjectionVulkanLHF(G_DEGREE_TO_RADIAN(65), aspect, 0.1, 100, projectionMatrix);
		LightDirection.x = -1.0f;
		LightDirection.y = -1.0f;
		LightDirection.z = 2.0f;
		lightColor.x = 0.9f;
		lightColor.y = 0.9f;
		lightColor.z = 1.0f;
		lightColor.w = 1.0f;
		sunAmbient.x = .25f;
		sunAmbient.y = .25f;
		sunAmbient.z = .35f;
		sunAmbient.w = 1.0f;


		modelData.viewMatrix = viewMatrix;
		modelData.projectionMatrix = projectionMatrix;
		modelData.DirectionalLightDirection[0] = LightDirection;
		modelData.DirectionalLightColor[0] = lightColor;
		modelData.DirectionalLightCount = 1;
		modelData.camEye = Eye;
		modelData.sunAmbient = sunAmbient;

		//modelData.matricies[0] = worldMatrix;
		//modelData.matricies[1] = rotationWorldMatrix;
		//modelData.materials[0] = FSLogo_materials[0].attrib;
		//modelData.materials[1] = FSLogo_materials[1].attrib;

		// TODO: Part 2b
		// TODO: Part 4g
		// TODO: part 3b

		/***************** GEOMETRY INTIALIZATION ******************/
		// Grab the device & physical device so we can allocate some stuff
		
		vlk.GetDevice((void**)&device);
		vlk.GetPhysicalDevice((void**)&physicalDevice);

		LevelVector.push_back("..\\FirstLevel.txt");
		LevelVector.push_back("..\\SecondLevel.txt");

		std::cout << "Loading First Level" << "\n";
		LevelObject = ParseLevel(LevelVector[LevelNumber]);
		LoadLevel(LevelObject, device, physicalDevice);
		SetupModelData(LevelObject, modelData);

		for (size_t i = 0; i < LevelObject.LevelLights.size(); i++)
		{
			modelData.PointLightColor[i] = LevelObject.LevelLights[i].LightColor;
			modelData.PointLightPosition[i] = LevelObject.LevelLights[i].LightPosition;
			modelData.PointLightFalloffs[i] = LevelObject.LevelLights[i].Falloff;
			modelData.PointLightCount += 1;
		}

		// TODO: Part 1c
		// Create Vertex Buffer
		float verts[] = {
				0.0, 0.5f,
				0.5f, -0.5f,
				-0.5, -0.5f
		};
		// Transfer triangle data to the vertex buffer. (staging would be prefered here)
		GvkHelper::create_buffer(physicalDevice, device, sizeof(FSLogo_vertices),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vertexHandle, &vertexData);
		GvkHelper::write_to_buffer(device, vertexData, FSLogo_vertices, sizeof(FSLogo_vertices));
		GvkHelper::create_buffer(physicalDevice, device, sizeof(FSLogo_indices),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &indexHandle, &indexData);
		GvkHelper::write_to_buffer(device, indexData, FSLogo_indices, sizeof(FSLogo_indices));
		// TODO: Part 1g
		// TODO: Part 2d

		unsigned max_frames = 0;
		vlk.GetSwapchainImageCount(max_frames);
		storageHandle.resize(max_frames);
		storageData.resize(max_frames);
		for (size_t i = 0; i < max_frames; i++)
		{
			GvkHelper::create_buffer(physicalDevice, device, sizeof(modelData),
				VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
				VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &storageHandle[i], &storageData[i]);
			GvkHelper::write_to_buffer(device, storageData[i], &modelData, sizeof(modelData));
		}

		/***************** SHADER INTIALIZATION ******************/
		// Intialize runtime shader compiler HLSL -> SPIRV
		shaderc_compiler_t compiler = shaderc_compiler_initialize();
		shaderc_compile_options_t options = shaderc_compile_options_initialize();
		shaderc_compile_options_set_source_language(options, shaderc_source_language_hlsl);
		shaderc_compile_options_set_invert_y(options, false); // TODO: Part 2i
#ifndef NDEBUG
		shaderc_compile_options_set_generate_debug_info(options);
#endif
		std::string vertexShaderSource = ShaderAsString("..\\VertexShader.hlsl");
		// Create Vertex Shader
		shaderc_compilation_result_t result = shaderc_compile_into_spv( // compile
			compiler, vertexShaderSource.c_str(), strlen(vertexShaderSource.c_str()),
			shaderc_vertex_shader, "main.vert", "main", options);
		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			std::cout << "Vertex Shader Errors: " << shaderc_result_get_error_message(result) << std::endl;
		/*	const char* szErrorMessage = shaderc_result_get_error_message(result);
			int nDebug = 0;*/
		}
		GvkHelper::create_shader_module(device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &vertexShader);
		shaderc_result_release(result); // done

		std::string pixelShaderSource = ShaderAsString("..\\PixelShader.hlsl");
		// Create Pixel Shader
		result = shaderc_compile_into_spv( // compile
			compiler, pixelShaderSource.c_str(), strlen(pixelShaderSource.c_str()),
			shaderc_fragment_shader, "main.frag", "main", options);
		if (shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success) // errors?
		{
			std::cout << "Pixel Shader Errors: " << shaderc_result_get_error_message(result) << std::endl;
			//const char* szErrorMessage = shaderc_result_get_error_message(result);
			//int nDebug = 0;
		}
		GvkHelper::create_shader_module(device, shaderc_result_get_length(result), // load into Vulkan
			(char*)shaderc_result_get_bytes(result), &pixelShader);
		shaderc_result_release(result); // done
		// Free runtime shader compiler resources
		shaderc_compile_options_release(options);
		shaderc_compiler_release(compiler);

		/***************** PIPELINE INTIALIZATION ******************/
		// Create Pipeline & Layout (Thanks Tiny!)
		VkRenderPass renderPass;
		vlk.GetRenderPass((void**)&renderPass);
		VkPipelineShaderStageCreateInfo stage_create_info[2] = {};
		// Create Stage Info for Vertex Shader
		stage_create_info[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage_create_info[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		stage_create_info[0].module = vertexShader;
		stage_create_info[0].pName = "main";
		// Create Stage Info for Fragment Shader
		stage_create_info[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		stage_create_info[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		stage_create_info[1].module = pixelShader;
		stage_create_info[1].pName = "main";
		// Assembly State
		VkPipelineInputAssemblyStateCreateInfo assembly_create_info = {};
		assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		assembly_create_info.primitiveRestartEnable = false;
		// TODO: Part 1e
		// Vertex Input State
		VkVertexInputBindingDescription vertex_binding_description = {};
		vertex_binding_description.binding = 0;
		vertex_binding_description.stride = sizeof(OBJ_VERT);
		vertex_binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		VkVertexInputAttributeDescription vertex_attribute_description[3] = {
			{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 },
			{ 1, 0, VK_FORMAT_R32G32B32_SFLOAT, 12 },
			{ 2, 0, VK_FORMAT_R32G32B32_SFLOAT, 24 }//uv, normal, etc....
		};
		VkPipelineVertexInputStateCreateInfo input_vertex_info = {};
		input_vertex_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		input_vertex_info.vertexBindingDescriptionCount = 1;
		input_vertex_info.pVertexBindingDescriptions = &vertex_binding_description;
		input_vertex_info.vertexAttributeDescriptionCount = 3;
		input_vertex_info.pVertexAttributeDescriptions = vertex_attribute_description;
		// Viewport State (we still need to set this up even though we will overwrite the values)
		VkViewport viewport = {
            0, 0, static_cast<float>(width), static_cast<float>(height), 0, 1
        };
        VkRect2D scissor = { {0, 0}, {width, height} };
		VkPipelineViewportStateCreateInfo viewport_create_info = {};
		viewport_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_create_info.viewportCount = 1;
		viewport_create_info.pViewports = &viewport;
		viewport_create_info.scissorCount = 1;
		viewport_create_info.pScissors = &scissor;
		// Rasterizer State
		VkPipelineRasterizationStateCreateInfo rasterization_create_info = {};
		rasterization_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterization_create_info.rasterizerDiscardEnable = VK_FALSE;
		rasterization_create_info.polygonMode = VK_POLYGON_MODE_FILL;
		rasterization_create_info.lineWidth = 1.0f;
		rasterization_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterization_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterization_create_info.depthClampEnable = VK_FALSE;
		rasterization_create_info.depthBiasEnable = VK_FALSE;
		rasterization_create_info.depthBiasClamp = 0.0f;
		rasterization_create_info.depthBiasConstantFactor = 0.0f;
		rasterization_create_info.depthBiasSlopeFactor = 0.0f;
		// Multisampling State
		VkPipelineMultisampleStateCreateInfo multisample_create_info = {};
		multisample_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisample_create_info.sampleShadingEnable = VK_FALSE;
		multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisample_create_info.minSampleShading = 1.0f;
		multisample_create_info.pSampleMask = VK_NULL_HANDLE;
		multisample_create_info.alphaToCoverageEnable = VK_FALSE;
		multisample_create_info.alphaToOneEnable = VK_FALSE;
		// Depth-Stencil State
		VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info = {};
		depth_stencil_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil_create_info.depthTestEnable = VK_TRUE;
		depth_stencil_create_info.depthWriteEnable = VK_TRUE;
		depth_stencil_create_info.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil_create_info.depthBoundsTestEnable = VK_FALSE;
		depth_stencil_create_info.minDepthBounds = 0.0f;
		depth_stencil_create_info.maxDepthBounds = 1.0f;
		depth_stencil_create_info.stencilTestEnable = VK_FALSE;
		// Color Blending Attachment & State
		VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
		color_blend_attachment_state.colorWriteMask = 0xF;
		color_blend_attachment_state.blendEnable = VK_FALSE;
		color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
		color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_DST_COLOR;
		color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_DST_ALPHA;
		color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
		VkPipelineColorBlendStateCreateInfo color_blend_create_info = {};
		color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blend_create_info.logicOpEnable = VK_FALSE;
		color_blend_create_info.logicOp = VK_LOGIC_OP_COPY;
		color_blend_create_info.attachmentCount = 1;
		color_blend_create_info.pAttachments = &color_blend_attachment_state;
		color_blend_create_info.blendConstants[0] = 0.0f;
		color_blend_create_info.blendConstants[1] = 0.0f;
		color_blend_create_info.blendConstants[2] = 0.0f;
		color_blend_create_info.blendConstants[3] = 0.0f;
		// Dynamic State 
		VkDynamicState dynamic_state[2] = { 
			// By setting these we do not need to re-create the pipeline on Resize
			VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamic_create_info = {};
		dynamic_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_create_info.dynamicStateCount = 2;
		dynamic_create_info.pDynamicStates = dynamic_state;
		
		// TODO: Part 2e
		// TODO: Part 2f
			// TODO: Part 4f
		// TODO: Part 2g
			// TODO: Part 4f
		// TODO: Part 2h
			// TODO: Part 4f
	
		VkDescriptorSetLayoutBinding LayoutBinding = { 0 };
		LayoutBinding.descriptorCount = 1;
		LayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		LayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		LayoutBinding.pImmutableSamplers = nullptr;
		LayoutBinding.binding = 0;
		VkDescriptorSetLayoutCreateInfo LayoutCreateInfo = {};
		LayoutCreateInfo.bindingCount = 1;
		LayoutCreateInfo.pBindings = &LayoutBinding;
		LayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		LayoutCreateInfo.pNext = nullptr;
		LayoutCreateInfo.flags = 0;
		VkResult resultX = vkCreateDescriptorSetLayout(device, &LayoutCreateInfo,
			nullptr, &descriptorLayout);

		VkDescriptorPoolCreateInfo descriptorPool_Create_Info = {};
		descriptorPool_Create_Info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		VkDescriptorPoolSize descriptorpool_size = { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, max_frames };
		descriptorPool_Create_Info.poolSizeCount = max_frames;
		descriptorPool_Create_Info.pPoolSizes = &descriptorpool_size;
		descriptorPool_Create_Info.maxSets = max_frames;
		descriptorPool_Create_Info.flags = 0;
		descriptorPool_Create_Info.pNext = nullptr;
		vkCreateDescriptorPool(device, &descriptorPool_Create_Info, nullptr, &descriptorPool);

		VkDescriptorSetAllocateInfo descriptorset_allocate_info = {};
		descriptorset_allocate_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorset_allocate_info.descriptorSetCount = 1;
		descriptorset_allocate_info.pSetLayouts = &descriptorLayout;
		descriptorset_allocate_info.descriptorPool = descriptorPool;
		descriptorset_allocate_info.pNext = nullptr;
		descriptorSet.resize(max_frames);
		for (int i = 0; i < max_frames; ++i) {
			vkAllocateDescriptorSets(device, &descriptorset_allocate_info, &descriptorSet[i]);
		}

		VkWriteDescriptorSet write_descriptorset = {};
		write_descriptorset.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptorset.descriptorCount = 1;
		write_descriptorset.dstArrayElement = 0;
		write_descriptorset.dstBinding = 0;
		write_descriptorset.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		for (int i = 0; i < max_frames; ++i) {
			write_descriptorset.dstSet = descriptorSet[i];
			VkDescriptorBufferInfo dbinfo = { storageHandle[i], 0, VK_WHOLE_SIZE };
			write_descriptorset.pBufferInfo = &dbinfo;
			vkUpdateDescriptorSets(device, 1, &write_descriptorset, 0, nullptr);
		}

		VkPushConstantRange constantRange;
		constantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		constantRange.offset = 0;
		constantRange.size = sizeof(unsigned int) * 2;

		// Descriptor pipeline layout
		VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
		pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// TODO: Part 2e
		pipeline_layout_create_info.setLayoutCount = 1;
		pipeline_layout_create_info.pSetLayouts = &descriptorLayout;
		// TODO: Part 3c
		pipeline_layout_create_info.pushConstantRangeCount = 1;
		pipeline_layout_create_info.pPushConstantRanges = &constantRange;
		vkCreatePipelineLayout(device, &pipeline_layout_create_info, 
			nullptr, &pipelineLayout);



	    // Pipeline State... (FINALLY) 
		VkGraphicsPipelineCreateInfo pipeline_create_info = {};
		pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_create_info.stageCount = 2;
		pipeline_create_info.pStages = stage_create_info;
		pipeline_create_info.pInputAssemblyState = &assembly_create_info;
		pipeline_create_info.pVertexInputState = &input_vertex_info;
		pipeline_create_info.pViewportState = &viewport_create_info;
		pipeline_create_info.pRasterizationState = &rasterization_create_info;
		pipeline_create_info.pMultisampleState = &multisample_create_info;
		pipeline_create_info.pDepthStencilState = &depth_stencil_create_info;
		pipeline_create_info.pColorBlendState = &color_blend_create_info;
		pipeline_create_info.pDynamicState = &dynamic_create_info;
		pipeline_create_info.layout = pipelineLayout;
		pipeline_create_info.renderPass = renderPass;
		pipeline_create_info.subpass = 0;
		pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
		vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, 
			&pipeline_create_info, nullptr, &pipeline);

		assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
			&pipeline_create_info, nullptr, &BoundingBoxesPipeline);

		/***************** CLEANUP / SHUTDOWN ******************/
		// GVulkanSurface will inform us when to release any allocated resources
		shutdown.Create(vlk, [&]() {
			if (+shutdown.Find(GW::GRAPHICS::GVulkanSurface::Events::RELEASE_RESOURCES, true)) {
				CleanUp(); // unlike D3D we must be careful about destroy timing
			}
		});
	}

	GW::MATH::GMATRIXF TempViewMatrix = GW::MATH::GIdentityMatrixF;

	void UpdateCamera()
	{
		//std::chrono::high_resolution_clock timer;
		TempViewMatrix = modelData.viewMatrix;
		ProxyMatrix.InverseF(TempViewMatrix, TempViewMatrix);
		GW::MATH::GVECTORF TranslatePosition = GW::MATH::GVECTORF();
		const float Camera_Speed = 5;

		float w = 0, a = 0, s = 0, d = 0, leftshift = 0, space = 0;
		float ControllerForwardBackward = 0, ControllerLeftRight = 0, LeftTrigger = 0, RightTrigger = 0;
		ProxyInput.GetState(G_KEY_W, w);
		ProxyInput.GetState(G_KEY_A, a);
		ProxyInput.GetState(G_KEY_S, s);
		ProxyInput.GetState(G_KEY_D, d);

		ProxyController.GetState(0, G_LY_AXIS, ControllerForwardBackward);
		ProxyController.GetState(0, G_LX_AXIS, ControllerLeftRight);

		TranslatePosition.z = ((w - s) + ControllerForwardBackward) * Camera_Speed * timer.Delta();
		TranslatePosition.x = ((d - a) + ControllerLeftRight) * Camera_Speed * timer.Delta();

		/*TranslatePosition.z -= (s + ControllerForwardBackward) * Camera_Speed * timer.Delta();
		TranslatePosition.z += (w + ControllerForwardBackward) * Camera_Speed * timer.Delta();
		TranslatePosition.x -= (a + ControllerLeftRight) * Camera_Speed * timer.Delta();
		TranslatePosition.x += (d + ControllerLeftRight) * Camera_Speed * timer.Delta();*/

		ProxyInput.GetState(G_KEY_LEFTSHIFT, leftshift);
		ProxyInput.GetState(G_KEY_SPACE, space);

		ProxyController.GetState(0, G_LEFT_TRIGGER_AXIS, LeftTrigger);
		ProxyController.GetState(0, G_RIGHT_TRIGGER_AXIS, RightTrigger);

		float YChange = (space * Camera_Speed) - (leftshift * Camera_Speed) + (RightTrigger * Camera_Speed) - (LeftTrigger * Camera_Speed);
		TranslatePosition.y = YChange * Camera_Speed * timer.Delta();
		ProxyMatrix.TranslateLocalF(TempViewMatrix, TranslatePosition, TempViewMatrix);
		GW::MATH::GVECTORF SavePosition = TempViewMatrix.row4;

		float MouseX = 0, MouseY = 0;
		float ControllerRotationX = 0, ControllerRotationY = 0;
		unsigned int ClientWidth = 0, ClientHeight = 0;
		win.GetClientWidth(ClientWidth);
		win.GetClientHeight(ClientHeight);

		const float Rotation_Speed = 0.05 + (pi * timer.Delta());

		GW::MATH::GVECTORF CameraRotation = GW::MATH::GVECTORF();

		GW::GReturn result = ProxyInput.GetMouseDelta(MouseX, MouseY);
		ProxyController.GetState(0, G_RX_AXIS, ControllerRotationX);
		ProxyController.GetState(0, G_RY_AXIS, ControllerRotationY);

		int ControllerIndex = 0;
		//ProxyController.GetMaxIndex(ControllerIndex);
		bool isConnected = false;
		GW::GReturn ControllerResult = ProxyController.IsConnected(ControllerIndex, isConnected);

		if (G_PASS(result) && result != GW::GReturn::REDUNDANT)
		{
			CameraRotation.x += 65 * aspect * (MouseX / ClientWidth) * Rotation_Speed;
			CameraRotation.y += 65 * (MouseY / ClientHeight) * Rotation_Speed;
		}

		if (G_PASS(ControllerResult) && ControllerResult != GW::GReturn::REDUNDANT)
		{
			CameraRotation.x += 65 * aspect * (ControllerRotationX / ClientWidth) * Rotation_Speed;
			CameraRotation.y += 65 * (-ControllerRotationY / ClientHeight) * Rotation_Speed;
		}


		GW::MATH::GMatrix::RotateYGlobalF(TempViewMatrix, CameraRotation.x, TempViewMatrix);
		GW::MATH::GMatrix::RotateXLocalF(TempViewMatrix, CameraRotation.y, TempViewMatrix);
		TempViewMatrix.row4 = SavePosition;

		ProxyMatrix.InverseF(TempViewMatrix, TempViewMatrix);

		modelData.viewMatrix = TempViewMatrix;

		unsigned int currentBuffer;
		vlk.GetSwapchainCurrentImage(currentBuffer);
		GvkHelper::write_to_buffer(device,
			storageData[currentBuffer], &modelData, sizeof(SHADER_MODEL_DATA));

	}

	std::vector<VkClearColorValue> SkyColor = { {0.125f, 0.066f, 0.078f, 1}, {0.019f, 0.007f, 0.070f, 1} };
	int ColorIndex = 0;

	void LevelSwap(Level &InputLevel, VkDevice& device, VkPhysicalDevice& physicalDevice)
	{
		ColorIndex++;
		if (ColorIndex > 1)
		{
			ColorIndex = 0;
		}
		clrAndDepth[0].color = SkyColor[ColorIndex];
		LevelCleanup(InputLevel, device);
		std::cout << "Loading" << LevelVector[LevelNumber] << "Level" << "\n";
		InputLevel = ParseLevel(LevelVector[LevelNumber]);
		LoadLevel(InputLevel, device, physicalDevice);
		SetupModelData(InputLevel, modelData);
		for (size_t i = 0; i < LevelObject.LevelLights.size(); i++)
		{
			modelData.PointLightColor[i] = LevelObject.LevelLights[i].LightColor;
			modelData.PointLightPosition[i] = LevelObject.LevelLights[i].LightPosition;
			modelData.PointLightFalloffs[i] = LevelObject.LevelLights[i].Falloff;
			modelData.PointLightCount += 1;
		}
	}

	XTime timer = XTime();

	void Update()
	{

		timer.Signal();
		UpdateCamera();
		
		if (GetAsyncKeyState(VK_F1) &0x8000)
		{
			LevelNumber++;
			if (LevelNumber >= LevelVector.size())
			{
				LevelNumber = 0;
			}

			LevelSwap(LevelObject, device, physicalDevice);
		}

		if (GetAsyncKeyState('R') & 0x1)
		{
			std::cout << "Reset Camera Position" << "\n";

			mathOperator.LookAtLHF(Eye, Look, Up, viewMatrix);

			modelData.viewMatrix = viewMatrix;

			unsigned int currentBuffer;
			vlk.GetSwapchainCurrentImage(currentBuffer);
			GvkHelper::write_to_buffer(device,
				storageData[currentBuffer], &modelData, sizeof(SHADER_MODEL_DATA));
		}

		if (GetAsyncKeyState('T') & 0x1)
		{

			LevelObject.TriangulateFlag = !LevelObject.TriangulateFlag;

			if(LevelObject.TriangulateFlag == true)
			{
				std::cout << "Toggled Triangulation On" << "\n";
			}
			else if(LevelObject.TriangulateFlag == false)
			{
				std::cout << "Toggled Triangulation Off" << "\n";
			}

		}

		if (GetAsyncKeyState('B') & 0x1)
		{
			LevelObject.BoundingBoxFlag = !LevelObject.BoundingBoxFlag;

			if (LevelObject.BoundingBoxFlag == true)
			{
				std::cout << "Toggled Bounding Boxes On" << "\n";
			}
			else if (LevelObject.BoundingBoxFlag == false)
			{
				std::cout << "Toggled Bounding Boxes Off" << "\n";
			}

		}

		// Adjust CPU data to reflect what we want to draw
			//GW::MATH::GMatrix::RotateYLocalF(modelData.matricies[1],
			//0.001f, modelData.matricies[1]);
		// Copy data to this frame's buffer
		//unsigned int currentBuffer;
		//vlk.GetSwapchainCurrentImage(currentBuffer);
		//GvkHelper::write_to_buffer(device,
		//	storageData[currentBuffer], &modelData, sizeof(SHADER_MODEL_DATA));
	}
	void Render()
	{
		// TODO: Part 2a
		// TODO: Part 4d
		// grab the current Vulkan commandBuffer
		unsigned int currentBuffer;
		vlk.GetSwapchainCurrentImage(currentBuffer);
		VkCommandBuffer commandBuffer;
		vlk.GetCommandBuffer(currentBuffer, (void**)&commandBuffer);
		// what is the current client area dimensions?
		unsigned int width, height;
		win.GetClientWidth(width);
		win.GetClientHeight(height);
		// setup the pipeline's dynamic settings
		VkViewport viewport = {
            0, 0, static_cast<float>(width), static_cast<float>(height), 0, 1
        };
        VkRect2D scissor = { {0, 0}, {width, height} };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
		
		if (LevelObject.TriangulateFlag)
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BoundingBoxesPipeline);
		}
		else if (LevelObject.BoundingBoxFlag)
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, BoundingBoxesPipeline);
		}
		else
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			
		}
		
		
		/*for (size_t i = 0; i < descriptorSet.size(); i++)
		{
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipelineLayout, 0, 1, &descriptorSet[i], 0, nullptr);
		}*/
		

		// now we can draw
		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexHandle, offsets);
		// TODO: Part 1h
		//vkCmdBindIndexBuffer(commandBuffer, indexHandle, *offsets, VK_INDEX_TYPE_UINT32);
		// TODO: Part 4d
		// TODO: Part 2i
		// TODO: Part 3b
			// TODO: Part 3d
		//for (size_t i = 0; i < FSLogo_meshcount; i++)
		//{
		//	vkCmdPushConstants(commandBuffer, pipelineLayout, (VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT), *offsets, sizeof(unsigned int), &i);
		//	vkCmdDrawIndexed(commandBuffer, FSLogo_meshes[i].indexCount, 1, FSLogo_meshes[i].indexOffset , *offsets, 0); // TODO: Part 1d, 1h
		//}

		RenderModels(LevelObject, descriptorSet, commandBuffer, pipelineLayout, offsets);
		
	}
	
private:
	void CleanUp()
	{
		// wait till everything has completed
		vkDeviceWaitIdle(device);
		// Release allocated buffers, shaders & pipeline
		for (int i = 0; i < storageData.size(); ++i) {
			vkDestroyBuffer(device, storageHandle[i], nullptr);
			vkFreeMemory(device, storageData[i], nullptr);
		}

		storageData.clear();
		storageHandle.clear();

		LevelCleanup(LevelObject, device);

		vkDestroyDescriptorSetLayout(device, descriptorLayout, nullptr);
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		// TODO: Part 1g
		// TODO: Part 2d
		vkDestroyBuffer(device, vertexHandle, nullptr);
		vkFreeMemory(device, vertexData, nullptr);
		vkDestroyShaderModule(device, vertexShader, nullptr);
		vkDestroyShaderModule(device, pixelShader, nullptr);
		// TODO: Part 2e
		// TODO: part 2f
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
		vkDestroyPipeline(device, pipeline, nullptr);
	}
};
