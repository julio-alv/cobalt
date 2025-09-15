cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

project(cobalt)

find_package(raylib REQUIRED)

add_subdirectory(vendor/glm)
add_subdirectory(vendor/entt)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glm::glm
        EnTT
        raylib
)

# Optional: For Web or desktop builds
# set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)