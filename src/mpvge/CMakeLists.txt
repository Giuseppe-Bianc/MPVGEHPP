add_executable(mpvge main.cpp)

target_link_libraries(
  mpvge
  PRIVATE MPVGEHPP::MPVGEHPP_options
          MPVGEHPP::MPVGEHPP_warnings)

target_link_system_libraries(
  mpvge
  PRIVATE
          CLI11::CLI11
          MPVGEHPP::mpvge_lib)

target_include_directories(mpvge PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
