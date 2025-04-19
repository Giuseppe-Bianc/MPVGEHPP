include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(MPVGEHPP_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(MPVGEHPP_setup_options)
  option(MPVGEHPP_ENABLE_HARDENING "Enable hardening" ON)
  option(MPVGEHPP_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    MPVGEHPP_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    MPVGEHPP_ENABLE_HARDENING
    OFF)

  MPVGEHPP_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR MPVGEHPP_PACKAGING_MAINTAINER_MODE)
    option(MPVGEHPP_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(MPVGEHPP_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(MPVGEHPP_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(MPVGEHPP_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(MPVGEHPP_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(MPVGEHPP_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(MPVGEHPP_ENABLE_PCH "Enable precompiled headers" OFF)
    option(MPVGEHPP_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(MPVGEHPP_ENABLE_IPO "Enable IPO/LTO" ON)
    option(MPVGEHPP_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(MPVGEHPP_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(MPVGEHPP_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(MPVGEHPP_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(MPVGEHPP_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(MPVGEHPP_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(MPVGEHPP_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(MPVGEHPP_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(MPVGEHPP_ENABLE_PCH "Enable precompiled headers" OFF)
    option(MPVGEHPP_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      MPVGEHPP_ENABLE_IPO
      MPVGEHPP_WARNINGS_AS_ERRORS
      MPVGEHPP_ENABLE_USER_LINKER
      MPVGEHPP_ENABLE_SANITIZER_ADDRESS
      MPVGEHPP_ENABLE_SANITIZER_LEAK
      MPVGEHPP_ENABLE_SANITIZER_UNDEFINED
      MPVGEHPP_ENABLE_SANITIZER_THREAD
      MPVGEHPP_ENABLE_SANITIZER_MEMORY
      MPVGEHPP_ENABLE_UNITY_BUILD
      MPVGEHPP_ENABLE_CLANG_TIDY
      MPVGEHPP_ENABLE_CPPCHECK
      MPVGEHPP_ENABLE_COVERAGE
      MPVGEHPP_ENABLE_PCH
      MPVGEHPP_ENABLE_CACHE)
  endif()

  MPVGEHPP_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (MPVGEHPP_ENABLE_SANITIZER_ADDRESS OR MPVGEHPP_ENABLE_SANITIZER_THREAD OR MPVGEHPP_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(MPVGEHPP_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(MPVGEHPP_global_options)
  if(MPVGEHPP_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    MPVGEHPP_enable_ipo()
  endif()

  MPVGEHPP_supports_sanitizers()

  if(MPVGEHPP_ENABLE_HARDENING AND MPVGEHPP_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR MPVGEHPP_ENABLE_SANITIZER_UNDEFINED
       OR MPVGEHPP_ENABLE_SANITIZER_ADDRESS
       OR MPVGEHPP_ENABLE_SANITIZER_THREAD
       OR MPVGEHPP_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${MPVGEHPP_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${MPVGEHPP_ENABLE_SANITIZER_UNDEFINED}")
    MPVGEHPP_enable_hardening(MPVGEHPP_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(MPVGEHPP_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(MPVGEHPP_warnings INTERFACE)
  add_library(MPVGEHPP_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  MPVGEHPP_set_project_warnings(
    MPVGEHPP_warnings
    ${MPVGEHPP_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(MPVGEHPP_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    MPVGEHPP_configure_linker(MPVGEHPP_options)
  endif()

  include(cmake/Sanitizers.cmake)
  MPVGEHPP_enable_sanitizers(
    MPVGEHPP_options
    ${MPVGEHPP_ENABLE_SANITIZER_ADDRESS}
    ${MPVGEHPP_ENABLE_SANITIZER_LEAK}
    ${MPVGEHPP_ENABLE_SANITIZER_UNDEFINED}
    ${MPVGEHPP_ENABLE_SANITIZER_THREAD}
    ${MPVGEHPP_ENABLE_SANITIZER_MEMORY})

  set_target_properties(MPVGEHPP_options PROPERTIES UNITY_BUILD ${MPVGEHPP_ENABLE_UNITY_BUILD})

  if(MPVGEHPP_ENABLE_PCH)
    target_precompile_headers(
      MPVGEHPP_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(MPVGEHPP_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    MPVGEHPP_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(MPVGEHPP_ENABLE_CLANG_TIDY)
    MPVGEHPP_enable_clang_tidy(MPVGEHPP_options ${MPVGEHPP_WARNINGS_AS_ERRORS})
  endif()

  if(MPVGEHPP_ENABLE_CPPCHECK)
    MPVGEHPP_enable_cppcheck(${MPVGEHPP_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(MPVGEHPP_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    MPVGEHPP_enable_coverage(MPVGEHPP_options)
  endif()

  if(MPVGEHPP_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(MPVGEHPP_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(MPVGEHPP_ENABLE_HARDENING AND NOT MPVGEHPP_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR MPVGEHPP_ENABLE_SANITIZER_UNDEFINED
       OR MPVGEHPP_ENABLE_SANITIZER_ADDRESS
       OR MPVGEHPP_ENABLE_SANITIZER_THREAD
       OR MPVGEHPP_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    MPVGEHPP_enable_hardening(MPVGEHPP_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
