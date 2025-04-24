/*
 * Created by gbian on 02/04/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix)
// clang-format on
#include "MPVGE/RenderPipeline.hpp"

#include "MPVGE/Model.hpp"
// #define INDEPTH

namespace mpvge {

    RenderPipeline::RenderPipeline(Device &devicein, const std::string &vertFilepath, const std::string &fragFilepath,
                                   const PipelineConfigInfo &configInfo)
      : device{devicein} {
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
    }

    RenderPipeline::~RenderPipeline() {
        auto deviceHandle = device.getDevice();
        DESTROY_VK_HANDLE(vertShaderModule, vkDestroyShaderModule(deviceHandle, vertShaderModule, nullptr));
        DESTROY_VK_HANDLE(fragShaderModule, vkDestroyShaderModule(deviceHandle, fragShaderModule, nullptr));
        DESTROY_VK_HANDLE(graphicsPipeline, vkDestroyPipeline(deviceHandle, graphicsPipeline, nullptr));
    }

    std::vector<char> RenderPipeline::readFile(const std::string &filepath) {
#ifdef INDEPTH
        vnd::AutoTimer timer{std::format("Read File {}", filepath)};
#endif

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if(!file) { throw std::runtime_error(FORMAT("Failed to open file: {}", filepath)); }

        const std::streamsize size = file.tellg();
        if(size <= 0) {
            return {};  // Return empty vector for empty or unreadable file
        }

        std::vector<char> buffer;
        buffer.resize(C_ST(size));

        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), size);

        if(!file) { throw std::runtime_error(FORMAT("Failed to read the complete file: {}", filepath)); }

        return buffer;
    }

    void RenderPipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath,
                                                const PipelineConfigInfo &configInfo) {
        assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no pipelineLayout provided in configInfo");
        assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no renderPass provided in configInfo");

        const auto vertCode = readFile(vertFilepath);
        const auto fragCode = readFile(fragFilepath);
        /*LINFO("Vertex Shader Code Size: {}", vertCode.size());
        LINFO("Fragment Shader Code Size: {}", fragCode.size());*/

        createShaderModule(vertCode, &vertShaderModule);
        createShaderModule(fragCode, &fragShaderModule);
        if(mpvge::DebugUtil::getInstance().isInitialized()) {
            mpvge::DebugUtil::getInstance().setObjectName(vertShaderModule, "Vertex Shader Module");
            mpvge::DebugUtil::getInstance().setObjectName(fragShaderModule, "Fragment Shader Module");
        }

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescriptions = Model::Vertex::getBindingDescriptions();
        auto attributeDescriptions = Model::Vertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = C_UI32T(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = C_UI32T(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = C_UI32T(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = configInfo.pipelineLayout;
        pipelineInfo.renderPass = configInfo.renderPass;
        pipelineInfo.subpass = configInfo.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        VK_CHECK(vkCreateGraphicsPipelines(device.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline),
                 "failed to create graphics pipeline");

        mpvge::DebugUtil::getInstance().setObjectNameIfinit(graphicsPipeline, "Graphics Pipeline");
    }

    void RenderPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = std::bit_cast<const uint32_t *>(code.data());

        VK_CHECK(vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, shaderModule), "failed to create shader module");
    }

    void RenderPipeline::bind(VkCommandBuffer commandBuffer) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        mpvge::DebugUtil::getInstance().setObjectNameIfinit(commandBuffer, "GP Command Buffer");
    }

    PipelineConfigInfo RenderPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height) {
        PipelineConfigInfo configInfo{};

        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = C_F(width);
        configInfo.viewport.height = C_F(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;

        configInfo.scissor.offset = {0, 0};
        configInfo.scissor.extent = {width, height};

        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                                         VK_COLOR_COMPONENT_A_BIT;
        configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.colorBlendInfo.attachmentCount = 1;
        configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
        configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional

        return configInfo;
    }

}  // namespace mpvge
   // clang-format off
// NOLINTEND(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix)
   // clang-format on