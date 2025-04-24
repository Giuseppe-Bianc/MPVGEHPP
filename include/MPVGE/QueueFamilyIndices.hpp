/*
 * Created by gbian on 31/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#pragma once

#include "headers.hpp"

namespace mpvge {

    class QueueFamilyIndices {
    public:
        QueueFamilyIndices() = default;

        void set_graphics_family(uint32_t index) noexcept;
        void set_present_family(uint32_t index) noexcept;

        [[nodiscard]] uint32_t graphics_family() const noexcept;
        [[nodiscard]] uint32_t present_family() const noexcept;
        [[nodiscard]] bool has_graphics_family() const noexcept;
        [[nodiscard]] bool has_present_family() const noexcept;
        [[nodiscard]] bool is_complete() const noexcept;

    private:
        uint32_t graphics_family_{};
        uint32_t present_family_{};
        bool graphics_family_has_value_{false};
        bool present_family_has_value_{false};
    };

}  // namespace mpvge
