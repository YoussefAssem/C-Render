cmake_minimum_required(VERSION 3.1)

project("C-Render" C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(external)
add_subdirectory(src)
