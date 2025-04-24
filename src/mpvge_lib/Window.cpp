/*
 * Created by gbian on 03/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-multiway-paths-covered, *-init-variables, *-qualified-auto)
#include <MPVGE/Window.hpp>

namespace mpvge {
    DISABLE_WARNINGS_PUSH(26432 26447)
    Window::Window(const int w, const int h, const std::string_view &window_name) noexcept : width(w), height(h), windowName(window_name) {
        initWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    DISABLE_WARNINGS_POP()

    void Window::initWindow() {
        initializeGLFW();
        setHints();
        enumenrateMonitors();
        createWindow();
        centerWindow();
    }

    void Window::createWindow() {
        vnd::AutoTimer timer("glfw_window creation");

        // Use std::unique_ptr with custom deleter
        // NOLINTNEXTLINE(*-suspicious-stringview-data-usage)
        window = glfwCreateWindow(width, height, windowName.data(), nullptr, nullptr);

        if(window == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }
        glfwSetKeyCallback(window, keyCallback);
    }

    void Window::enumenrateMonitors() {
        vnd::Timer monitort("get primary Monitor");
        auto monitors = Monitor::enumerateMonitors();
        if(monitors.empty()) { throw std::runtime_error("Failed to enumerate monitors."); }
        for(const auto &monitor : monitors) {
            if(monitor.getMonitorInfo().isPrimary) {
                primaryMonitor = monitor;
                break;
            }
        }
        LINFO("{}", monitort);
    }

    void Window::setHints() const noexcept {
        vnd::AutoTimer timer("set glfw hints");
        // Set GLFW context version and profile
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }
    void Window::initializeGLFW() {
        vnd::AutoTimer timer("glfw setup");
        if(!glfwInit()) {
            LCRITICAL("Failed to initialize GLFW");
            throw std::runtime_error("Failed to initialize GLFW.");
        }
        if(!glfwVulkanSupported()) {
            glfwTerminate();
            LCRITICAL("Failed to initialize GLFW. Vulkan not supported");
            throw std::runtime_error("Failed to initialize GLFW. Vulkan not supported");
        }
        glfwSetErrorCallback(errorCallback);
    }

    void Window::centerWindow() {
        vnd::Timer crepositiont("calculating for reposition");
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        auto centerX = CALC_CENTRO(primaryMonitor.getMonitorInfo().width, windowWidth);
        auto centerY = CALC_CENTRO(primaryMonitor.getMonitorInfo().height, windowHeight);
        LINFO("{}", crepositiont);

#ifndef __linux__
        vnd::Timer wrepositiont("window reposition");
        glfwSetWindowPos(window, centerX, centerY);
        int posX = 0;
        int posY = 0;
        glfwGetWindowPos(window, &posX, &posY);
        if(posX != centerX || posY != centerY) { throw std::runtime_error("Failed to position the window at the center."); }
        LINFO("{}", wrepositiont);
#endif

        glfwSetWindowUserPointer(window, this);
        // glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwShowWindow(window);
        LINFO("Monitor: {}", primaryMonitor.getMonitorInfo());
        LINFO("Created the window {0}: (w: {1}, h: {2}, pos:({3}/{4}))", windowName.data(), width, height, centerX, centerY);
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, surface), "failed to craete window surface");
    }

    /*void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height) noexcept {
        auto lveWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
        lveWindow->framebufferResized = true;
        lveWindow->width = width;
        lveWindow->height = height;
    }*/
}  // namespace mpvge

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-multiway-paths-covered, *-init-variables, *-qualified-auto)
