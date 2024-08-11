cmake_minimum_required(VERSION 3.10)
project(HotReloadExample)

add_subdirectory(src/plugin)

add_executable(main src/main.c)
target_link_libraries(main dl)  # Link with `dl` for dynamic loading on Linux
