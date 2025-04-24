/*
 * Created by derbinan on 14/03/25.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-uppercase-literal-suffix,*-uppercase-literal-suffix)
#include "MPVGE/Monitor.hpp"

namespace mpvge {
    Monitor::Monitor(GLFWmonitor *monitorin, bool isPrimary) : monitor(monitorin) {
        if(!monitor) { throw std::runtime_error("Failed to get the primary monitor."); }
        fetchMonitorInfo(isPrimary);
    }
    void Monitor::fetchMonitorInfo(bool isPrimary) {
        mode = glfwGetVideoMode(monitor);
        if(!mode) { throw std::runtime_error("Failed to get video mode."); }
        info.width = mode->width;
        info.height = mode->height;
        info.redBits = mode->redBits;
        info.greenBits = mode->greenBits;
        info.blueBits = mode->blueBits;
        info.refreshRate = mode->refreshRate;
        info.isPrimary = isPrimary;
        info.name = glfwGetMonitorName(monitor);
        glfwGetMonitorPos(monitor, &info.xPos, &info.yPos);
        glfwGetMonitorContentScale(monitor, &info.scaleX, &info.scaleY);
        glfwGetMonitorPhysicalSize(monitor, &info.physicalWidth, &info.physicalHeight);
    }
    void Monitor::setCustomMode(const GLFWvidmode *customMode) {
        if(!customMode) { throw std::runtime_error("Failed to get custom video mode."); }
        mode = customMode;
        info.width = mode->width;
        info.height = mode->height;
        info.redBits = mode->redBits;
        info.greenBits = mode->greenBits;
        info.blueBits = mode->blueBits;
        info.refreshRate = mode->refreshRate;
    }
    std::vector<Monitor> Monitor::enumerateMonitors() {
        std::vector<Monitor> monitorList;
        int count = 0;
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if(!primaryMonitor) { throw std::runtime_error("Failed to get the primary monitor."); }
        monitorList.emplace_back(primaryMonitor, true);
        GLFWmonitor **monitors = glfwGetMonitors(&count);

        for(int i = 0; i < count; ++i) {
            // Salta il primary monitor per evitare duplicati.
            if(monitors[i] == primaryMonitor) { continue; }
            monitorList.emplace_back(monitors[i]);
        }
        return monitorList;
    }

}  // namespace mpvge

// NOLINTEND(*-include-cleaner, *-uppercase-literal-suffix,*-uppercase-literal-suffix)
