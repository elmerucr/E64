add_library(core STATIC core.cpp)

target_link_libraries(core blitter lua timer)
