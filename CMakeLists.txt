cmake_minimum_required(VERSION 3.20)

project(DarthEx VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

add_library(darthex_domain INTERFACE)
target_include_directories(darthex_domain INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_features(darthex_domain INTERFACE cxx_std_17)

add_executable(darthexc src/main.cpp)
target_link_libraries(darthexc PRIVATE darthex_domain)
darthex_enable_warnings(darthexc)
set_target_properties(darthexc PROPERTIES CXX_EXTENSIONS OFF)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
