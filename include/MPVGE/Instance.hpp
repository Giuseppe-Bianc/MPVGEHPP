/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "headers.hpp"
#include "vulkanCheck.hpp"

namespace mpvge {

    class Instance {
    public:
        bool checkValidationLayerSupport();
        Instance(const char *appName, bool enableValidationLayersin);
        ~Instance();
        Instance(const Instance &) = delete;
        Instance &operator=(const Instance &) = delete;
        VkInstance get() const { return instance; }

    private:
        void createInstance(const char *app_name);
        std::vector<const char *> getRequiredExtensions() const;
        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                     const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
        static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                  const VkAllocationCallbacks *pAllocator);
        void setupDebugMessenger();
        void hasGflwRequiredInstanceExtensions(const std::vector<const char *> &requiredExtensions) const;
        VkInstance instance{VK_NULL_HANDLE};
        bool enableValidationLayers;
        VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
    };

}  // namespace mpvge

// NOLINTEND(*-include-cleaner)