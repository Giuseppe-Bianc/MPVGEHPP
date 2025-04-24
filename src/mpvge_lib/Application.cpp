/*
 * Created by gbian on 29/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-pro-type-member-init,*-member-init, *-avoid-magic-numbers,*-magic-numbers, *-uppercase-literal-suffix, *-use-designated-initializers, *-pro-type-union-access)
// clang-format on
#include "MPVGE/Application.hpp"
#include "MPVGE/FPSCounter.hpp"

namespace mpvge {
    Application::Application() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }
    Application::~Application() { DESTROY_VK_HANDLE(pipelineLayout, vkDestroyPipelineLayout(device.getDevice(), pipelineLayout, nullptr)); }

    void Application::run() {
        // NOLINTNEXTLINE(*-suspicious-stringview-data-usage)
        FPSCounter fpsCounter{window.getGLFWWindow(), wtile.data()};
        while(!window.shouldClose()) {
            glfwPollEvents();
            fpsCounter.frameInTitle(false, false);
            [[maybe_unused]] auto frameTime = C_F(fpsCounter.getFrameTime());
            drawFrame();
        }

        vkDeviceWaitIdle(device.getDevice());
    }

    void Application::loadModels() {
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        model = std::make_unique<Model>(device, vertices);
    }

    void Application::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        VK_CHECK(vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout),
                 "failed to create pipeline layout!");
        mpvge::DebugUtil::getInstance().setObjectNameIfinit(pipelineLayout, "Pipeline Layout");
    }
    void Application::createPipeline() {
        auto pipelineConfig = RenderPipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
        pipelineConfig.renderPass = lveSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<RenderPipeline>(
            device, calculateRelativePathToShaders(curentP, "simple_shader.vert.opt.rmp.spv").string(),
            calculateRelativePathToShaders(curentP, "simple_shader.frag.opt.rmp.spv").string(), pipelineConfig);
    }
    void Application::createCommandBuffers() {
        commandBuffers.resize(lveSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        VK_CHECK(vkAllocateCommandBuffers(device.getDevice(), &allocInfo, commandBuffers.data()), "failed to allocate command buffers!");
        mpvge::DebugUtil::getInstance().setObjectNames(commandBuffers, "Command Buffers");
        for(std::size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            VK_CHECK(vkBeginCommandBuffer(commandBuffers[i], &beginInfo), "failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = lveSwapChain.getRenderPass();
            renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(C_I(i));

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {{0.1f, 0.1f, 0.1f, 1.0f}};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = C_UI32T(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            lvePipeline->bind(commandBuffers[i]);
            model->bind(commandBuffers[i]);
            model->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            VK_CHECK(vkEndCommandBuffer(commandBuffers[i]), "failed to record command buffer!");
        }
    }

    void Application::drawFrame() {
        uint32_t imageIndex = 0;
        auto result = lveSwapChain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) { throw std::runtime_error("failed to acquire swap chain image!"); }

        result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        VK_CHECK(result, "failed to present swap chain image!");
    }
}  // namespace mpvge
   // clang-format off
// NOLINTEND(*-include-cleaner, *-pro-type-member-init,*-member-init, *-avoid-magic-numbers,*-magic-numbers, *-uppercase-literal-suffix, *-use-designated-initializers, *-pro-type-union-access)
   // clang-format on