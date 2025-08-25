cmake_minimum_required(VERSION 3.16)
project(core LANGUAGES CXX)

file(GLOB_RECURSE SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(core ${SRC})

target_include_directories(core 
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/indexbuffer
        ${CMAKE_CURRENT_SOURCE_DIR}/vertexbuffer
        ${CMAKE_CURRENT_SOURCE_DIR}/shader
        ${CMAKE_CURRENT_SOURCE_DIR}/window
)

target_link_libraries(core
    PRIVATE
        glad
        glfw
)