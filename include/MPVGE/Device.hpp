/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "DebugUtil.hpp"
#include "DeviceInfo.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "headers.hpp"

namespace mpvge {

    class Device {
    public:
        Device(Instance &instancein, Surface &surfacein, bool enableValidationLayersin);
        ~Device();
        Device(const Device &) = delete;
        Device(Device &&) = delete;
        Device &operator=(const Device &) = delete;
        Device &operator=(Device &&) = delete;

        [[nodiscard]] VkDevice getDevice() const noexcept { return device; }
        [[nodiscard]] VkPhysicalDeviceProperties getProperties() const noexcept { return properties; }
        [[nodiscard]] VkCommandPool getCommandPool() const noexcept { return commandPool; }
        [[nodiscard]] VkSurfaceKHR getSurface() const noexcept { return surface.get(); }
        [[nodiscard]] VkQueue getGraphicsQueue() const noexcept { return graphicsQueue; }
        [[nodiscard]] VkQueue getPresentQueue() const noexcept { return presentQueue; }

        SwapChainSupportDetails getSwapChainSupport() const { return surface.querySwapChainSupport(physicalDevice); }
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags improperties);
        QueueFamilyIndices findPhysicalQueueFamilies() const { return surface.getQueueFamilyIndices(physicalDevice); }
        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags improperties, VkBuffer &buffer,
                          VkDeviceMemory &bufferMemory);
        VkCommandBuffer beginSingleTimeCommands() noexcept;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) noexcept;
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) noexcept;
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) noexcept;

        void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags improperties, VkImage &image,
                                 VkDeviceMemory &imageMemory);
        void cmdBeginLabel(VkCommandBuffer commandBuffer, const char *labelName, const std::vector<float> &color) noexcept;
        void cmdInsertLabel(VkCommandBuffer commandBuffer, const char *labelName, const std::vector<float> &color) noexcept;
        void cmdEndLabel(VkCommandBuffer commandBuffer) noexcept;
        void queueBeginLabel(VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept;
        void queueInsertLabel(VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept;
        void queueEndLabel(VkQueue queue) noexcept;

    private:
        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        void createLogicalDevice(const QueueFamilyIndices &indices);
        void createCommandPool(const QueueFamilyIndices &queueFamilyIndices);
        void pcmdBeginLabel(VkInstance instancein, VkCommandBuffer commandBuffer, const char *labelName,
                            const std::vector<float> &color) noexcept;
        void pcmdInsertLabel(VkInstance instancein, VkCommandBuffer commandBuffer, const char *labelName,
                             const std::vector<float> &color) noexcept;
        void pcmdEndLabel(VkInstance instancein, VkCommandBuffer commandBuffer) noexcept;
        void pqueueBeginLabel(VkInstance instancein, VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept;
        void pqueueInsertLabel(VkInstance instancein, VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept;
        void pqueueEndLabel(VkInstance instancein, VkQueue queue) noexcept;
        bool enableValidationLayers;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkCommandPool commandPool;
        VkPhysicalDeviceProperties properties;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        Surface &surface;
        Instance &instance;
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    };

}  // namespace mpvge

// NOLINTEND(*-include-cleaner)