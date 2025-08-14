project(stb)

add_library(stb include/stb_image.h include/stb_image.c)
target_include_directories(stb PUBLIC include/)