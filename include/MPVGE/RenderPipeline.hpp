/*
 * Created by gbian on 02/04/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)
#pragma once

#include "Device.hpp"
#include "headers.hpp"

namespace mpvge {
    struct PipelineConfigInfo {
        PipelineConfigInfo() = default;
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class RenderPipeline {
    public:
        RenderPipeline(Device &devicein, const std::string &vertFilepath, const std::string &fragFilepath,
                       const PipelineConfigInfo &configInfo);
        ~RenderPipeline();

        RenderPipeline(const RenderPipeline &) = delete;
        void operator=(const RenderPipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filepath);

        void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        Device &device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };

}  // namespace mpvge

// NOLINTEND(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)