// NOLINTBEGIN(*-include-cleaner)

#include <CLI/CLI.hpp>
#include <MPVGE/mpvge_lib.hpp>
#include <internal_use_only/config.hpp>

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv) {
    INIT_LOG();
    try {
        CLI::App app{FORMAT("{} version {}", MPVGE::cmake::project_name, MPVGE::cmake::project_version)};

        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        CLI11_PARSE(app, argc, argv);

        if(show_version) {
            fmt::print("{}\n", MPVGE::cmake::project_version);
            return EXIT_SUCCESS;
        }
        mpvge::Application app_instance;
        app_instance.run();
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}

// NOLINTEND(*-include-cleaner)