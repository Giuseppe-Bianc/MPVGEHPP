/*
 * Created by gbian on 04/03/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers,*-magic-numbers)
#include "MPVGE/FPSCounter.hpp"

FPSCounter::FPSCounter(GLFWwindow *window, std::string_view title) noexcept
  : last_time(vnd::clock::now()), frames(0), fps(0.0L), ms_per_frame(0.0L), m_window(window), m_title(title) {}

std::string FPSCounter::isVsync(bool vsync) { return {vsync ? "Enabled" : "Disabled"}; }
void FPSCounter::frame(bool vsync, [[maybe_unused]] bool showMax) {
    updateFPS();
    ms_per_frameComposition = transformTime(ms_per_frame);
    auto vsyncStr = isVsync(vsync);
    auto uifps = C_UI32T(fps);
    LINFO("{} fps/{} - VSync: {}", uifps, ms_per_frameComposition, vsyncStr);
}

void FPSCounter::frameInTitle(bool vsync, [[maybe_unused]] bool showMax) {
    updateFPS();
    ms_per_frameComposition = transformTime(ms_per_frame);
    auto vsyncStr = isVsync(vsync);
    auto uifps = C_UI32T(fps);
    glfwSetWindowTitle(m_window, FORMATST("{} - {} fps/{} - VSync: {}", m_title, uifps, ms_per_frameComposition, vsyncStr).c_str());
}

DISABLE_WARNINGS_PUSH(26447)
std::string FPSCounter::transformTime(const long double inputTimeMilli) noexcept {
    using namespace std::chrono;

    const auto durationmils = duration<long double, std::milli>(inputTimeMilli);

    // Use floor to extract whole milliseconds and microseconds.
    const auto durationMs = floor<milliseconds>(durationmils);
    const auto remainderAfterMs = durationmils - durationMs;
    const auto durationUs = floor<microseconds>(remainderAfterMs);
    const auto remainderAfterUs = remainderAfterMs - durationUs;
    // Round the final nanoseconds from the remaining fraction.
    const auto durationNs = round<nanoseconds>(remainderAfterUs);

    return FORMAT("{}ms,{}us,{}ns", durationMs.count(), durationUs.count(), durationNs.count());
}
DISABLE_WARNINGS_POP()

void FPSCounter::updateFPS() noexcept {
    const auto current_time = vnd::clock::now();
    frames++;
    const auto ldframes = C_LD(frames);
    const auto time_step = vnd::TimeValues(ch::duration_cast<vnd::nanolld>(current_time - last_time).count());
    last_time = current_time;
    frameTime = time_step.get_seconds();
    totalTime += frameTime;

    if(totalTime >= 1.0L) {
        fps = ldframes / totalTime;
        ms_per_frame = time_step.get_millis() / ldframes;
        frames = 0;
        totalTime = 0;
        // max_fps = std::max(max_fps, fps);
    }
}

long double FPSCounter::getFPS() const noexcept { return fps; }

long double FPSCounter::getMsPerFrame() const noexcept { return ms_per_frame; }

// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers,*-magic-numbers)
