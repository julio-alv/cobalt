add_library(core
    window/window.hpp
    window/window.cpp
)

target_include_directories(core PUBLIC window/)

target_include_directories(core
    PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/vendor/glad
        ${CMAKE_SOURCE_DIR}/vendor/glfw
)

target_link_libraries(core
    PRIVATE
        glad
        glfw
)