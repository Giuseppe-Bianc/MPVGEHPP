/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix)
// clang-format on
#include "MPVGE/Device.hpp"

namespace mpvge {
    static inline constexpr float queuePriority = 1.0f;
    // NOLINTBEGIN(*-make-member-function-const, *-pro-bounds-array-to-pointer-decay, *-no-array-decay)
    // Wrapper per vkCmdBeginDebugUtilsLabelEXT
    void Device::pcmdBeginLabel(VkInstance instancein, VkCommandBuffer commandBuffer, const char *labelName,
                                const std::vector<float> &color) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instancein, "vkCmdBeginDebugUtilsLabelEXT"));
        if(func != nullptr) {
            VkDebugUtilsLabelEXT label{};
            label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
            label.pLabelName = labelName;
            memcpy(label.color, color.data(), sizeof(float) * 4);
            func(commandBuffer, &label);
        }
    }

    // Wrapper per vkCmdInsertDebugUtilsLabelEXT
    void Device::pcmdInsertLabel(VkInstance instancein, VkCommandBuffer commandBuffer, const char *labelName,
                                 const std::vector<float> &color) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instancein, "vkCmdInsertDebugUtilsLabelEXT"));
        if(func != nullptr) {
            VkDebugUtilsLabelEXT label{};
            label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
            label.pLabelName = labelName;
            memcpy(label.color, color.data(), sizeof(float) * 4);
            func(commandBuffer, &label);
        }
    }

    // Wrapper per vkCmdEndDebugUtilsLabelEXT
    void Device::pcmdEndLabel(VkInstance instancein, VkCommandBuffer commandBuffer) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instancein, "vkCmdEndDebugUtilsLabelEXT"));
        if(func != nullptr) { func(commandBuffer); }
    }

    // Wrapper per vkQueueBeginDebugUtilsLabelEXT
    void Device::pqueueBeginLabel(VkInstance instancein, VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkQueueBeginDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instancein, "vkQueueBeginDebugUtilsLabelEXT"));
        if(func != nullptr) {
            VkDebugUtilsLabelEXT label{};
            label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
            label.pLabelName = labelName;
            memcpy(label.color, color.data(), sizeof(float) * 4);
            func(queue, &label);
        }
    }

    // Wrapper per vkQueueInsertDebugUtilsLabelEXT
    void Device::pqueueInsertLabel(VkInstance instancein, VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkQueueInsertDebugUtilsLabelEXT>(
            vkGetInstanceProcAddr(instancein, "vkQueueInsertDebugUtilsLabelEXT"));
        if(func != nullptr) {
            VkDebugUtilsLabelEXT label{};
            label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
            label.pLabelName = labelName;
            memcpy(label.color, color.data(), sizeof(float) * 4);
            func(queue, &label);
        }
    }

    // Wrapper per vkQueueEndDebugUtilsLabelEXT
    void Device::pqueueEndLabel(VkInstance instancein, VkQueue queue) noexcept {
        if(!enableValidationLayers) { return; }
        auto func = std::bit_cast<PFN_vkQueueEndDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instancein, "vkQueueEndDebugUtilsLabelEXT"));
        if(func != nullptr) { func(queue); }
    }

    void Device::cmdBeginLabel(VkCommandBuffer commandBuffer, const char *labelName, const std::vector<float> &color) noexcept {
        pcmdBeginLabel(instance.get(), commandBuffer, labelName, color);
    }
    void Device::cmdEndLabel(VkCommandBuffer commandBuffer) noexcept { pcmdEndLabel(instance.get(), commandBuffer); }
    void Device::cmdInsertLabel(VkCommandBuffer commandBuffer, const char *labelName, const std::vector<float> &color) noexcept {
        pcmdInsertLabel(instance.get(), commandBuffer, labelName, color);
    }
    void Device::queueBeginLabel(VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept {
        pqueueBeginLabel(instance.get(), queue, labelName, color);
    }

    void Device::queueEndLabel(VkQueue queue) noexcept { pqueueEndLabel(instance.get(), queue); }
    void Device::queueInsertLabel(VkQueue queue, const char *labelName, const std::vector<float> &color) noexcept {
        pqueueInsertLabel(instance.get(), queue, labelName, color);
    }
    // NOLINTEND(*-make-member-function-const, *-pro-bounds-array-to-pointer-decay, *-no-array-decay)

    Device::Device(Instance &instancein, Surface &surfacein, bool enableValidationLayersin)
      : enableValidationLayers{enableValidationLayersin}, surface{surfacein}, instance{instancein} {
        pickPhysicalDevice();
        const QueueFamilyIndices indices = surface.getQueueFamilyIndices(physicalDevice);
        createLogicalDevice(indices);
        createCommandPool(indices);
        auto instanceHandle = instance.get();
        debugUtilInitialize(instanceHandle, device);
        if(mpvge::DebugUtil::getInstance().isInitialized()) {
            mpvge::DebugUtil::getInstance().setObjectName(instanceHandle, "Instance");
            mpvge::DebugUtil::getInstance().setObjectName(surface.get(), "Surface");
            mpvge::DebugUtil::getInstance().setObjectName(device, "Logical Device");
            mpvge::DebugUtil::getInstance().setObjectName(physicalDevice, "Physical Device");
            mpvge::DebugUtil::getInstance().setObjectName(graphicsQueue, "Graphics Queue");
            mpvge::DebugUtil::getInstance().setObjectName(presentQueue, "Present Queue");
            mpvge::DebugUtil::getInstance().setObjectName(commandPool, "Command Pool");
        }
    }

    Device::~Device() {
        DESTROY_VK_HANDLE(commandPool, vkDestroyCommandPool(device, commandPool, nullptr));
        DESTROY_VK_HANDLE(device, vkDestroyDevice(device, nullptr));
    }

    void Device::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        auto instanceHandle = instance.get();
        vkEnumeratePhysicalDevices(instanceHandle, &deviceCount, nullptr);
        if(deviceCount == 0) { throw std::runtime_error("failed to find GPUs with Vulkan support!"); }
        LINFO("Device count: {}", deviceCount);
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instanceHandle, &deviceCount, devices.data());

        std::size_t deviceIndex = 0;
        for(const auto &[index, pdevice] : devices | std::views::enumerate) {
            if(isDeviceSuitable(pdevice)) {
                physicalDevice = pdevice;
                deviceIndex = C_ST(index);
                break;
            }
        }
        if(physicalDevice == VK_NULL_HANDLE) { throw std::runtime_error("failed to find a suitable GPU!"); }
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        VLINFO("Device index: {}", deviceIndex);
        printDeviceInfo(physicalDevice, properties);
    }

    bool Device::isDeviceSuitable(VkPhysicalDevice devicein) {
        QueueFamilyIndices indices = surface.getQueueFamilyIndices(devicein);
        const bool extensionsSupported = checkDeviceExtensionSupport(devicein);
        bool swapChainAdequate = false;
        if(extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = surface.querySwapChainSupport(devicein);
            swapChainAdequate = swapChainSupport.isAdequate();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(devicein, &supportedFeatures);
        return indices.is_complete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    bool Device::checkDeviceExtensionSupport(VkPhysicalDevice devicein) {
        uint32_t extensionCount{};
        vkEnumerateDeviceExtensionProperties(devicein, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(devicein, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for(const auto &extension : availableExtensions) {
            // NOLINTNEXTLINE(*-pro-bounds-array-to-pointer-decay, *-no-array-decay)
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    void Device::createLogicalDevice(const QueueFamilyIndices &indices) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphics_family(), indices.present_family()};

        for(uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.emplace_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = C_UI32T(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = C_UI32T(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // might not really be necessary anymore because device specific validation layers
        // have been deprecated
#ifdef NDEBUG
        if(enableValidationLayers) [[unlikely]] {
            createInfo.enabledLayerCount = NC_UI32T(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else [[likely]] {
            createInfo.enabledLayerCount = 0;
        }
#else
        if(enableValidationLayers) [[likely]] {
            createInfo.enabledLayerCount = NC_UI32T(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else [[unlikely]] {
            createInfo.enabledLayerCount = 0;
        }
#endif

        VK_CHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device), "failed to create logical device!");

        vkGetDeviceQueue(device, indices.graphics_family(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.present_family(), 0, &presentQueue);
    }

    void Device::createCommandPool(const QueueFamilyIndices &queueFamilyIndices) {
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphics_family();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        VK_CHECK(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool), "failed to create command pool!");
    }

    void Device::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags improperties, VkBuffer &buffer,
                              VkDeviceMemory &bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VK_CHECK(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer), "failed to create vertex buffer!");

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, improperties);

        VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory), "failed to allocate vertex buffer memory!");
        if(mpvge::DebugUtil::getInstance().isInitialized()) {
            mpvge::DebugUtil::getInstance().setObjectName(buffer, "Vertex Buffer");
            mpvge::DebugUtil::getInstance().setObjectName(bufferMemory, "Vertex Buffer Memory");
        }

        vkBindBufferMemory(device, buffer, bufferMemory, 0);
    }
    VkCommandBuffer Device::beginSingleTimeCommands() noexcept {
        auto instanceHandle = instance.get();
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer{};
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        mpvge::DebugUtil::getInstance().setObjectNameIfinit(commandBuffer, "Command Buffer");

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);
        pcmdBeginLabel(instanceHandle, commandBuffer, "Begin Single Time Commands", {0.0f, 1.0f, 0.0f, 1.0f});

        return commandBuffer;
    }

    void Device::endSingleTimeCommands(VkCommandBuffer commandBuffer) noexcept {
        auto instanceHandle = instance.get();
        // Chiude la label inserita
        pcmdEndLabel(instanceHandle, commandBuffer);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        pqueueBeginLabel(instanceHandle, graphicsQueue, "Submit Single Time Command", {1.0f, 1.0f, 1.0f, 1.0f});
        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(graphicsQueue);
        pqueueEndLabel(instanceHandle, graphicsQueue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

    void Device::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) noexcept {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;  // Optional
        copyRegion.dstOffset = 0;  // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

        endSingleTimeCommands(commandBuffer);
    }

    void Device::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) noexcept {
        VkCommandBuffer commandBuffer = beginSingleTimeCommands();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;

        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = layerCount;

        // NOLINTBEGIN(*-use-designated-initializers)
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};
        // NOLINTEND(*-use-designated-initializers)

        vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
        endSingleTimeCommands(commandBuffer);
    }

    void Device::createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags improperties, VkImage &image,
                                     VkDeviceMemory &imageMemory) {
        VK_CHECK(vkCreateImage(device, &imageInfo, nullptr, &image), "failed to create image!");

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, improperties);

        VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory), "failed to allocate image memory!");
        VK_CHECK(vkBindImageMemory(device, image, imageMemory, 0), "failed to bind image memory!");
        if(mpvge::DebugUtil::getInstance().isInitialized()) {
            mpvge::DebugUtil::getInstance().setObjectName(image, "Image");
            mpvge::DebugUtil::getInstance().setObjectName(imageMemory, "Image Memory");
        }
    }

    uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags improperties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        const std::bitset<32> typeBits(typeFilter);
        for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if(typeBits.test(i) && (memProperties.memoryTypes[i].propertyFlags & improperties) == improperties) { return i; }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    VkFormat Device::findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for(const VkFormat &format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            // NOLINTBEGIN(*-branch-clone)
            // clang-format off
            if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
            // clang-format on
            // NOLINTEND(*-branch-clone)
        }
        throw std::runtime_error("failed to find supported format!");
    }
}  // namespace mpvge

// clang-format off
// NOLINTEND(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix)
// clang-format on
