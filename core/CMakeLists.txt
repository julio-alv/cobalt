add_library(core
    indexbuffer/indexbuffer.h
    indexbuffer/indexbuffer.cpp
    renderer/renderer.h
    renderer/renderer.cpp
    shader/shader.h
    shader/shader.cpp
    texture/texture.h
    texture/texture.cpp
    vertexarray/vertexarray.h
    vertexarray/vertexarray.cpp
    vertexbuffer/vertexbuffer.h
    vertexbuffer/vertexbuffer.cpp
    vertexlayout/vertexlayout.h
    vertexlayout/vertexlayout.cpp
    window/window.h
    window/window.cpp
)

target_include_directories(core PUBLIC
    indexbuffer
    renderer
    shader
    texture
    vertexarray
    vertexbuffer
    vertexlayout
    window
)

target_include_directories(core 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/window
        ${CMAKE_CURRENT_SOURCE_DIR}/renderer
)

target_link_libraries(core
    PRIVATE
        glad
        glfw
        glm::glm
        stb
)