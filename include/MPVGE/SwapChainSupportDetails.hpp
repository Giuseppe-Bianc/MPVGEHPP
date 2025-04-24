/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "headers.hpp"

namespace mpvge {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
        [[nodiscard]] bool isAdequate() const noexcept;
    };

}  // namespace mpvge

// NOLINTEND(*-include-cleaner)