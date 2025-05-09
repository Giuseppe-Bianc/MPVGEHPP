/*
 * Created by gbian on 03/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "Monitor.hpp"
#include "WindowCallback.hpp"
#include "headers.hpp"
#include "vulkanCheck.hpp"

namespace mpvge {

    // static void framebufferResizeCallback(GLFWwindow *window, int width, int height) noexcept;
    // NOLINT(*-special-member-functions)
    class Window {
    public:
        Window(const int w, const int h, const std::string_view &window_name) noexcept;
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        [[nodiscard]] GLFWwindow *getGLFWWindow() const noexcept { return window; }
        [[nodiscard]] bool shouldClose() const noexcept { return glfwWindowShouldClose(window); }
        [[nodiscard]] bool wasWindowResized() noexcept { return framebufferResized; }
        void resetWindowResizedFlag() noexcept { framebufferResized = false; }
        // void swapBuffers() const noexcept { glfwSwapBuffers(window); }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        VkExtent2D getExtent() noexcept { return {C_UI32T(width), C_UI32T(height)}; }

        static void initializeGLFW();

    private:
        void enumenrateMonitors();
        void initWindow();

        void createWindow();

        void setHints() const noexcept;

        void centerWindow();
        // static void framebufferResizeCallback(GLFWwindow *window, int width, int height) noexcept;

        int width;
        int height;
        bool framebufferResized = false;
        std::string_view windowName;
        Monitor primaryMonitor;
        GLFWwindow *window{nullptr};
    };
}  // namespace mpvge

// NOLINTEND(*-include-cleaner)
