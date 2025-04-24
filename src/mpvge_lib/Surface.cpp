/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// // NOLINTBEGIN(*-include-cleaner, *-signed-bitwise)
#include "MPVGE/Surface.hpp"

namespace mpvge {
    Surface::Surface(Instance &instancein, Window &window) : instance{instancein} { window.createWindowSurface(instance.get(), &surface); }
    Surface::~Surface() { DESTROY_VK_HANDLE(surface, vkDestroySurfaceKHR(instance.get(), surface, nullptr)); }
    QueueFamilyIndices Surface::getQueueFamilyIndices(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for(const auto &[i, queueFamily] : queueFamilies | std::views::enumerate) {
            if(queueFamily.queueCount > 0 && C_BOOL(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                indices.set_graphics_family(C_UI32T(i));
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, C_UI32T(i), surface, &presentSupport);
            if(queueFamily.queueCount > 0 && C_BOOL(presentSupport)) { indices.set_present_family(C_UI32T(i)); }
            if(indices.is_complete()) { break; }
        }

        return indices;
    }
    SwapChainSupportDetails Surface::querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount{};
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if(formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount{};
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if(presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }
        return details;
    }

}  // namespace mpvge

// NOLINTEND(*-include-cleaner, *-signed-bitwise)
