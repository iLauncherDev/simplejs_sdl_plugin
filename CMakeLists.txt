cmake_minimum_required(VERSION 3.24)
project(simplejs_sdl_plugin C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(SIMPLEJS_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/simplejs_src")
set(SIMPLEJS_SDK_INCLUDE_PATH "${SIMPLEJS_BASE_DIR}/sdk/include")

set(SIMPLEJS_DISABLE_TEST_PLUGIN ON)

include(${SIMPLEJS_BASE_DIR}/sdk/cmake/default.cmake)
include(${SIMPLEJS_BASE_DIR}/simplejs.cmake)

include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main
)

set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)

FetchContent_MakeAvailable(SDL3)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sdl3_plugin)
