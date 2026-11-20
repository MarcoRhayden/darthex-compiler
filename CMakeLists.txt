cmake_minimum_required(VERSION 3.20)

project(DarthEx VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

add_library(darthex_domain STATIC
  src/domain/keyword.cpp
)
target_include_directories(darthex_domain PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_compile_features(darthex_domain PUBLIC cxx_std_17)
darthex_enable_warnings(darthex_domain)
set_target_properties(darthex_domain PROPERTIES CXX_EXTENSIONS OFF)

add_library(darthex_application STATIC
  src/application/services/LexService.cpp
)
target_include_directories(darthex_application PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(darthex_application PUBLIC darthex_domain)
target_compile_features(darthex_application PUBLIC cxx_std_17)
darthex_enable_warnings(darthex_application)
set_target_properties(darthex_application PROPERTIES CXX_EXTENSIONS OFF)

add_library(darthex_adapters STATIC
  src/adapters/Lexer.cpp
  src/adapters/SymbolTable.cpp
  src/adapters/ConsoleReporter.cpp
)
target_include_directories(darthex_adapters PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(darthex_adapters PUBLIC darthex_application)
target_compile_features(darthex_adapters PUBLIC cxx_std_17)
darthex_enable_warnings(darthex_adapters)
set_target_properties(darthex_adapters PROPERTIES CXX_EXTENSIONS OFF)

add_executable(darthexc src/main.cpp)
target_link_libraries(darthexc PRIVATE darthex_adapters darthex_application)
darthex_enable_warnings(darthexc)
set_target_properties(darthexc PROPERTIES CXX_EXTENSIONS OFF)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
