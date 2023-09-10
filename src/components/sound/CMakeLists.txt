add_subdirectory(resid-0.16/)

add_library(sound STATIC sound.cpp analog.cpp)

target_link_libraries(sound resid)
