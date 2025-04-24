/*
 * Created by gbian on 29/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#pragma once
#include "headers.hpp"

#include "Device.hpp"
#include "Instance.hpp"
#include "Model.hpp"
#include "RenderPipeline.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "Window.hpp"

namespace mpvge {
    class Application {
    public:
        Application();
        ~Application();
        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;
        void run();

    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        const bool enableValidationLayers = true;
#endif
        Window window{wwidth, wheight, wtile};
        Instance instance{wtile.data(), enableValidationLayers};
        Surface surface{instance, window};
        Device device{instance, surface, enableValidationLayers};
        SwapChain lveSwapChain{device, window.getExtent()};
        std::unique_ptr<RenderPipeline> lvePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    };

}  // namespace mpvge
