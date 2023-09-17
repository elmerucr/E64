cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O3")

project(E64)

find_package(sdl2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    src/
    src/host/
    src/hud/
    src/lua-5.4.6/src/
    src/components/
    src/components/blitter/
    src/components/core/
    src/components/sound/
    src/components/sound/resid-0.16/
)

add_subdirectory(src/)

add_executable(E64 src/main.cpp)

target_link_libraries(E64 blitter core host hud sound ${SDL2_LIBRARIES})
