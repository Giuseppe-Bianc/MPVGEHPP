/*
 * Created by derbinan on 14/03/25.
 * Copyright (c) 2025 All rights reserved.
 */
// // NOLINTBEGIN(*-include-cleaner, *-uppercase-literal-suffix,*-uppercase-literal-suffix)
#pragma once

#include "headers.hpp"

namespace mpvge {
    // Struttura per raggruppare le informazioni del monitor.
    struct MonitorInfo {
        int width = 0, height = 0;
        int physicalWidth = 0, physicalHeight = 0;
        float scaleX = 0.0f, scaleY = 0.0f;
        int xPos = 0, yPos = 0;
        int redBits = 0, greenBits = 0, blueBits = 0;
        int refreshRate = 0;
        bool isPrimary = false;
        std::string name;
        [[nodiscard]] std::string to_string() const {
            return FORMAT("{:?} ({}x{}) Phys:{}x{}mm Pos: ({}, {}) Scl: ({}, {}) rgb{}{}{}, RR:{}Hz", name, width, height, physicalWidth,
                          physicalHeight, xPos, yPos, scaleX, scaleY, redBits, greenBits, blueBits, refreshRate);
        }
    };

    class Monitor {
    public:
        Monitor() = default;
        explicit Monitor(GLFWmonitor *monitorin, bool isPrimary = false);
        void fetchMonitorInfo(bool isPrimary);
        void setCustomMode(const GLFWvidmode *customMode);
        static std::vector<Monitor> enumerateMonitors();
        // Accessore per ottenere tutte le informazioni del monitor.
        const MonitorInfo &getMonitorInfo() const noexcept { return info; }

    private:
        GLFWmonitor *monitor = nullptr;
        const GLFWvidmode *mode = nullptr;
        MonitorInfo info;
    };
}  // namespace mpvge

template <> struct fmt::formatter<mpvge::MonitorInfo> : fmt::formatter<std::string_view> {
    auto format(const mpvge::MonitorInfo &monitor_info, format_context &ctx) const -> format_context::iterator {
        return fmt::formatter<std::string_view>::format(monitor_info.to_string(), ctx);
    }
};

// NOLINTEND(*-include-cleaner, *-uppercase-literal-suffix,*-uppercase-literal-suffix)