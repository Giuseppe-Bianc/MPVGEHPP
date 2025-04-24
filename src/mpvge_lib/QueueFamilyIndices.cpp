/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "MPVGE/QueueFamilyIndices.hpp"

namespace mpvge {
    void QueueFamilyIndices::set_graphics_family(uint32_t index) noexcept {
        graphics_family_ = index;
        graphics_family_has_value_ = true;
    }

    void QueueFamilyIndices::set_present_family(uint32_t index) noexcept {
        present_family_ = index;
        present_family_has_value_ = true;
    }

    uint32_t QueueFamilyIndices::graphics_family() const noexcept { return graphics_family_; }

    uint32_t QueueFamilyIndices::present_family() const noexcept { return present_family_; }

    bool QueueFamilyIndices::has_graphics_family() const noexcept { return graphics_family_has_value_; }

    bool QueueFamilyIndices::has_present_family() const noexcept { return present_family_has_value_; }

    bool QueueFamilyIndices::is_complete() const noexcept { return has_graphics_family() && has_present_family(); }
}  // namespace mpvge

// NOLINTEND(*-include-cleaner)