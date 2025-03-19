cmake_minimum_required(VERSION 3.30)
project(SimpleGrid LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Dependencies
add_subdirectory(vendor)

# SDL
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(SimpleGrid)
target_sources(SimpleGrid PRIVATE
        transform.c
        camera.c
        shader.c
        debug_grid.c
        main.c)
target_link_libraries(SimpleGrid PRIVATE glad SDL3::SDL3 cglm m)
