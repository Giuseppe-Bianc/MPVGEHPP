/*
 * Created by gbian on 08/04/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)
#pragma once

#include "headers.hpp"

namespace mpvge {

    class DebugUtil {
    public:
        static DebugUtil &getInstance() {
            static DebugUtil instance;
            return instance;
        }

        void init(VkInstance instance, VkDevice device) {
            m_device = device;
            m_instance = instance;
            m_setObjectNameFunc = std::bit_cast<PFN_vkSetDebugUtilsObjectNameEXT>(
                vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
        }

        [[nodiscard]] bool isInitialized() const { return m_device != VK_NULL_HANDLE; }

        template <typename T> void setObjectNameIfinit(T object, const std::string &name) const;
        template <typename T> void setObjectName(T object, const std::string &name) const;
        template <typename T> void setObjectNames(const std::vector<T> &object, const std::string &name) const;
        template <typename T> void setObjectNamesm(const std::vector<T> &object, const std::string &name) const;

    private:
        DebugUtil() = default;
        VkDevice m_device{VK_NULL_HANDLE};
        VkInstance m_instance{VK_NULL_HANDLE};
        PFN_vkSetDebugUtilsObjectNameEXT m_setObjectNameFunc = nullptr;

        template <typename T> static constexpr VkObjectType getObjectType();
    };

    template <typename T> void DebugUtil::setObjectNameIfinit(T object, const std::string &name) const {
        if(isInitialized()) { setObjectName(object, name); }
    }

    template <typename T> void DebugUtil::setObjectName(T object, const std::string &name) const {
        constexpr VkObjectType objectType = getObjectType<T>();
        if constexpr(objectType != VK_OBJECT_TYPE_UNKNOWN) {
            VkDebugUtilsObjectNameInfoEXT s{VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr, objectType, BC_UI64T(object),
                                            name.c_str()};
            if(!m_setObjectNameFunc) { return; }
            m_setObjectNameFunc(m_device, &s);
        }
    }
    template <typename T> void DebugUtil::setObjectNames(const std::vector<T> &object, const std::string &name) const {
        if(isInitialized()) { setObjectNamesm(object, name); }
    }
    template <typename T> void DebugUtil::setObjectNamesm(const std::vector<T> &object, const std::string &name) const {
        for(const auto &[index, objectHandle] : object | std::views::enumerate) {
            setObjectName(objectHandle, FORMAT("{} {}", name, index));
        }
    }

    // NOLINTBEGIN(*-function-cognitive-complexity)
    template <typename T> constexpr VkObjectType DebugUtil::getObjectType() {
        if constexpr(std::is_same_v<T, VkInstance>) {
            return VK_OBJECT_TYPE_INSTANCE;
        } else if constexpr(std::is_same_v<T, VkPhysicalDevice>) {
            return VK_OBJECT_TYPE_PHYSICAL_DEVICE;
        } else if constexpr(std::is_same_v<T, VkDevice>) {
            return VK_OBJECT_TYPE_DEVICE;
        } else if constexpr(std::is_same_v<T, VkQueue>) {
            return VK_OBJECT_TYPE_QUEUE;
        } else if constexpr(std::is_same_v<T, VkSemaphore>) {
            return VK_OBJECT_TYPE_SEMAPHORE;
        } else if constexpr(std::is_same_v<T, VkCommandBuffer>) {
            return VK_OBJECT_TYPE_COMMAND_BUFFER;
        } else if constexpr(std::is_same_v<T, VkFence>) {
            return VK_OBJECT_TYPE_FENCE;
        } else if constexpr(std::is_same_v<T, VkDeviceMemory>) {
            return VK_OBJECT_TYPE_DEVICE_MEMORY;
        } else if constexpr(std::is_same_v<T, VkBuffer>) {
            return VK_OBJECT_TYPE_BUFFER;
        } else if constexpr(std::is_same_v<T, VkImage>) {
            return VK_OBJECT_TYPE_IMAGE;
        } else if constexpr(std::is_same_v<T, VkEvent>) {
            return VK_OBJECT_TYPE_EVENT;
        } else if constexpr(std::is_same_v<T, VkQueryPool>) {
            return VK_OBJECT_TYPE_QUERY_POOL;
        } else if constexpr(std::is_same_v<T, VkBufferView>) {
            return VK_OBJECT_TYPE_BUFFER_VIEW;
        } else if constexpr(std::is_same_v<T, VkImageView>) {
            return VK_OBJECT_TYPE_IMAGE_VIEW;
        } else if constexpr(std::is_same_v<T, VkShaderModule>) {
            return VK_OBJECT_TYPE_SHADER_MODULE;
        } else if constexpr(std::is_same_v<T, VkPipelineCache>) {
            return VK_OBJECT_TYPE_PIPELINE_CACHE;
        } else if constexpr(std::is_same_v<T, VkPipelineLayout>) {
            return VK_OBJECT_TYPE_PIPELINE_LAYOUT;
        } else if constexpr(std::is_same_v<T, VkRenderPass>) {
            return VK_OBJECT_TYPE_RENDER_PASS;
        } else if constexpr(std::is_same_v<T, VkPipeline>) {
            return VK_OBJECT_TYPE_PIPELINE;
        } else if constexpr(std::is_same_v<T, VkDescriptorSetLayout>) {
            return VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
        } else if constexpr(std::is_same_v<T, VkSampler>) {
            return VK_OBJECT_TYPE_SAMPLER;
        } else if constexpr(std::is_same_v<T, VkDescriptorPool>) {
            return VK_OBJECT_TYPE_DESCRIPTOR_POOL;
        } else if constexpr(std::is_same_v<T, VkDescriptorSet>) {
            return VK_OBJECT_TYPE_DESCRIPTOR_SET;
        } else if constexpr(std::is_same_v<T, VkFramebuffer>) {
            return VK_OBJECT_TYPE_FRAMEBUFFER;
        } else if constexpr(std::is_same_v<T, VkCommandPool>) {
            return VK_OBJECT_TYPE_COMMAND_POOL;
        } else if constexpr(std::is_same_v<T, VkSamplerYcbcrConversion>) {
            return VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION;
        } else if constexpr(std::is_same_v<T, VkDescriptorUpdateTemplate>) {
            return VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE;
        } else if constexpr(std::is_same_v<T, VkPrivateDataSlot>) {
            return VK_OBJECT_TYPE_PRIVATE_DATA_SLOT;
#ifdef VK_KHR_surface
        } else if constexpr(std::is_same_v<T, VkSurfaceKHR>) {
            return VK_OBJECT_TYPE_SURFACE_KHR;
#endif
#ifdef VK_KHR_swapchain
        } else if constexpr(std::is_same_v<T, VkSwapchainKHR>) {
            return VK_OBJECT_TYPE_SWAPCHAIN_KHR;
#endif
#ifdef VK_KHR_display
        } else if constexpr(std::is_same_v<T, VkDisplayKHR>) {
            return VK_OBJECT_TYPE_DISPLAY_KHR;
        } else if constexpr(std::is_same_v<T, VkDisplayModeKHR>) {
            return VK_OBJECT_TYPE_DISPLAY_MODE_KHR;
#endif
#ifdef VK_EXT_debug_report
        } else if constexpr(std::is_same_v<T, VkDebugReportCallbackEXT>) {
            return VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT;
#endif
#ifdef VK_KHR_video_queue
        } else if constexpr(std::is_same_v<T, VkVideoSessionKHR>) {
            return VK_OBJECT_TYPE_VIDEO_SESSION_KHR;
        } else if constexpr(std::is_same_v<T, VkVideoSessionParametersKHR>) {
            return VK_OBJECT_TYPE_VIDEO_SESSION_PARAMETERS_KHR;
#endif
#ifdef VK_NVX_binary_import
        } else if constexpr(std::is_same_v<T, VkCuModuleNVX>) {
            return VK_OBJECT_TYPE_CU_MODULE_NVX;
        } else if constexpr(std::is_same_v<T, VkCuFunctionNVX>) {
            return VK_OBJECT_TYPE_CU_FUNCTION_NVX;
#endif
#ifdef VK_EXT_debug_utils
        } else if constexpr(std::is_same_v<T, VkDebugUtilsMessengerEXT>) {
            return VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT;
#endif
#ifdef VK_KHR_acceleration_structure
        } else if constexpr(std::is_same_v<T, VkAccelerationStructureKHR>) {
            return VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR;
#endif
#ifdef VK_EXT_validation_cache
        } else if constexpr(std::is_same_v<T, VkValidationCacheEXT>) {
            return VK_OBJECT_TYPE_VALIDATION_CACHE_EXT;
#endif
#ifdef VK_NV_ray_tracing
        } else if constexpr(std::is_same_v<T, VkAccelerationStructureNV>) {
            return VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV;
#endif
#ifdef VK_INTEL_performance_query
        } else if constexpr(std::is_same_v<T, VkPerformanceConfigurationINTEL>) {
            return VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL;
#endif
#ifdef VK_KHR_deferred_host_operations
        } else if constexpr(std::is_same_v<T, VkDeferredOperationKHR>) {
            return VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR;
#endif
#ifdef VK_NV_device_generated_commands
        } else if constexpr(std::is_same_v<T, VkIndirectCommandsLayoutNV>) {
            return VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV;
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
        } else if constexpr(std::is_same_v<T, VkCuModuleNV>) {
            return VK_OBJECT_TYPE_CUDA_MODULE_NV;
        } else if constexpr(std::is_same_v<T, VkCuFunctionNV>) {
            return VK_OBJECT_TYPE_CUDA_FUNCTION_NV;
#endif
#ifdef VK_FUCHSIA_buffer_collection
        } else if constexpr(std::is_same_v<T, VkBufferCollectionFUCHSIA>) {
            return VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA;
#endif
#ifdef VK_EXT_opacity_micromap
        } else if constexpr(std::is_same_v<T, VkMicromapEXT>) {
            return VK_OBJECT_TYPE_MICROMAP_EXT;
#endif
#ifdef VK_NV_optical_flow
        } else if constexpr(std::is_same_v<T, VkOpticalFlowSessionNV>) {
            return VK_OBJECT_TYPE_OPTICAL_FLOW_SESSION_NV;
#endif
#ifdef VK_EXT_shader_object
        } else if constexpr(std::is_same_v<T, VkShaderEXT>) {
            return VK_OBJECT_TYPE_SHADER_EXT;
#endif
#ifdef VK_NV_external_compute_queue
        } else if constexpr(std::is_same_v<T, VkExternalComputeQueueNV>) {
            return VK_OBJECT_TYPE_EXTERNAL_COMPUTE_QUEUE_NV;
#endif
#ifdef VK_EXT_device_generated_commands
        } else if constexpr(std::is_same_v<T, VkIndirectCommandsLayoutEXT>) {
            return VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_EXT;
        } else if constexpr(std::is_same_v<T, VkIndirectExecutionSetEXT>) {
            return VK_OBJECT_TYPE_INDIRECT_EXECUTION_SET_EXT;
#endif
        } else {
            return VK_OBJECT_TYPE_UNKNOWN;
        }
    }
    // NOLINTEND(*-function-cognitive-complexity)
}  // namespace mpvge

inline void debugUtilInitialize(VkInstance instance, VkDevice device) { mpvge::DebugUtil::getInstance().init(instance, device); }
// NOLINTEND(*-include-cleaner, *-pro-type-member-init, *-member-init, *-pro-type-cstyle-cast, *-pro-type-cstyle-cast)