add_library(core
    shader/shader.hpp
    shader/shader.cpp

    mesh/mesh.hpp
    mesh/mesh.cpp

    model/model.hpp
    model/model.cpp

    camera/camera.hpp
    camera/camera.cpp
)

target_include_directories(core PUBLIC shader/)
target_include_directories(core PUBLIC mesh/)
target_include_directories(core PUBLIC model/)
target_include_directories(core PUBLIC camera/)

target_include_directories(core
    PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/vendor/glad
        ${CMAKE_SOURCE_DIR}/vendor/glm
        ${CMAKE_SOURCE_DIR}/vendor/stb_image
        ${CMAKE_SOURCE_DIR}/vendor/tinygltf
)

target_link_libraries(core
    PRIVATE
        stb
        glad
        glm::glm
        tinygltf
)