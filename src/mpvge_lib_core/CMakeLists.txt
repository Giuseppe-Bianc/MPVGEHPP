include(GenerateExportHeader)


find_package(glm REQUIRED)
add_library(mpvge_lib_core mpvge_lib_core.cpp)



add_library(MPVGEHPP::mpvge_lib_core ALIAS mpvge_lib_core)

# Retrieve the target name
get_target_property(target_name mpvge_lib_core NAME)
include("${CMAKE_SOURCE_DIR}/cmake/Simd.cmake")
set_simd_instructions(${target_name})


target_link_libraries(mpvge_lib_core
        PRIVATE
        MPVGEHPP_options
        MPVGEHPP_warnings
        PUBLIC
        fmt::fmt
        spdlog::spdlog
        glm::glm
)

target_include_directories(mpvge_lib_core
        ${WARNING_GUARD} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
if (Vandior_BUILD_FUZZ_TESTS)
  target_link_libraries(mpvge_lib_core PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(mpvge_lib_core PRIVATE -fsanitize=fuzzer-no-link)
endif ()

target_compile_features(mpvge_lib_core PUBLIC cxx_std_${CMAKE_CXX_STANDARD})


set_target_properties(
        mpvge_lib_core
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(mpvge_lib_core EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/Vulktrt/Vulktrt/mpvge_lib_core_export.hpp)

if (NOT BUILD_SHARED_LIBS)
  target_compile_definitions(mpvge_lib_core PUBLIC OGL_LIB_STATIC_DEFINE)
endif ()
