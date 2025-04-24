/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#include "MPVGE/SwapChainSupportDetails.hpp"

namespace mpvge {
    bool SwapChainSupportDetails::isAdequate() const noexcept { return !formats.empty() && !presentModes.empty(); }

}  // namespace mpvge