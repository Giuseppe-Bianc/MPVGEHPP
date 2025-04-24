/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix, *-pro-bounds-array-to-pointer-decay,*-no-array-decay)
// clang-format on
#include "MPVGE/Instance.hpp"

#include "MPVGE/VulkanLogInfoCallback.hpp"

namespace mpvge {
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        [[maybe_unused]] void *pUserData) {
#ifdef INDEPTH
        vnd::AutoTimer t{"Debug Callback", vnd::Timer::Big};
#endif
        // Determine the message type
        const std::string type = VkDebugUtilsMessageTypeFlagsEXTString(messageType);

        // Format and log the message
        const auto msg = FORMAT("{}Message ID: {}({}): {}", type, pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "Unknown",
                                pCallbackData->messageIdNumber, pCallbackData->pMessage);

        printMessageWhitSeverity(msg, messageSeverity);

        logDebugValidationLayerInfo(pCallbackData, messageSeverity);

        return VK_FALSE;
    }

    bool Instance::checkValidationLayerSupport() {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char *layerName : validationLayers) {
            bool layerFound = false;
            for(const auto &layerProperties : availableLayers) {
                if(strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if(!layerFound) {
                LERROR("Validation layer {}  not supported!", layerName);
                return false;
            }
        }
        return true;
    }

    Instance::Instance(const char *appName, bool enableValidationLayersin) : enableValidationLayers(enableValidationLayersin) {
        createInstance(appName);
        setupDebugMessenger();
    }

    Instance::~Instance() {
        if(enableValidationLayers) { DESTROY_VK_HANDLE(debugMessenger, DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr)); }
        DESTROY_VK_HANDLE(instance, vkDestroyInstance(instance, nullptr));
    }
    void Instance::createInstance(const char *app_name) {
        if(enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("Validation layers requested but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = app_name;
        appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        const auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = C_UI32T(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef NDEBUG
        if(enableValidationLayers) [[unlikely]] {
            createInfo.enabledLayerCount = NC_UI32T(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else [[likely]] {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }
#else
        if(enableValidationLayers) [[likely]] {
            createInfo.enabledLayerCount = NC_UI32T(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else [[unlikely]] {
            createInfo.enabledLayerCount = 0;
            createInfo.ppEnabledLayerNames = nullptr;
        }
#endif

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create instance");

        hasGflwRequiredInstanceExtensions(extensions);
    }

    std::vector<const char *> Instance::getRequiredExtensions() const {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if(enableValidationLayers) { extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }
        return extensions;
    }

    void Instance::setupDebugMessenger() {
        if(!enableValidationLayers) { return; }

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;

        VK_CHECK(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger), "Failed to set up debug messenger!");
    }

    VkResult Instance::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                    const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = std::bit_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        if(func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }
    void Instance::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                 const VkAllocationCallbacks *pAllocator) {
        auto func = std::bit_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(func != nullptr) { func(instance, debugMessenger, pAllocator); }
    }

    void Instance::hasGflwRequiredInstanceExtensions(const std::vector<const char *> &requiredExtensions) const {
#ifdef INDEPTH
        vnd::AutoTimer t{"hasGflwRequiredInstanceExtensions", vnd::Timer::Big};
#endif
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::unordered_set<std::string_view> available;
        available.reserve(extensionCount);  // Riserviamo spazio per migliorare le prestazioni

#if defined(_DEBUG) || defined(DEBUG)
        std::vector<std::string> availableExtensions;
#endif
        available.reserve(extensionCount);
        for(const auto &[extensionName, specVersion] : extensions) {
#if defined(_DEBUG) || defined(DEBUG)
            availableExtensions.emplace_back(FORMAT("{} (v. {})", extensionName, specVersion));
#endif
            available.emplace(extensionName);
        }

        if(!std::ranges::all_of(requiredExtensions, [&](const auto &required) { return available.contains(required); })) [[unlikely]] {
            throw std::runtime_error("Missing required GLFW extension");
        }
#ifdef NDEBUG
        VLINFO("available extensions count: {}", available.size());
        VLINFO("required extensions count: {}", requiredExtensions.size());
#else
        VLINFO("available extensions:\n  {}", FMT_JOIN(availableExtensions, "\n  "));
        VLINFO("required extensions:\n  {}", FMT_JOIN(requiredExtensions, "\n  "));
#endif
    }
}  // namespace mpvge

// clang-format off
// NOLINTEND(*-include-cleaner, *-signed-bitwise, *-easily-swappable-parameters, *-use-anonymous-namespace, *-diagnostic-old-style-cast, *-pro-type-cstyle-cast, *-pro-type-member-init,*-member-init, *-pro-bounds-constant-array-index, *-qualified-auto, *-uppercase-literal-suffix, *-pro-bounds-array-to-pointer-decay,*-no-array-decay)
// clang-format on