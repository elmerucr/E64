add_library(blitter STATIC blitter.cpp blitter_terminal.cpp)

target_link_libraries(blitter rom)
