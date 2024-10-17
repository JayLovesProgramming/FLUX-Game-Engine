cmake_minimum_required(VERSION 3.10)
project(FLUXEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the directory paths for GLFW, GLAD, GLM, and Assimp
set(GLFW_INCLUDE_DIR "C:/glfw/include")
set(GLFW_LIBRARY_DIR "C:/glfw/lib-vc2022")

set(GLAD_INCLUDE_DIR "C:/glad/include")

set(GLM_INCLUDE_DIR "C:/glm")

set(ASSIMP_INCLUDE_DIR "C:/Program Files/Assimp/include")
set(ASSIMP_LIBRARY_DIR "C:/Program Files/Assimp/lib/x64")

# Include directories
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} "src")

# Link directories
link_directories(${GLFW_LIBRARY_DIR} ${ASSIMP_LIBRARY_DIR})

set(GLAD_SRC "C:/glad/src/glad.c")

# /Utils
file(GLOB UTILS_SOURCES src/Utils/*.cpp)
# /Shader
file(GLOB_RECURSE SHADER_SOURCES src/Shader/*.cpp)
# /Input
file(GLOB_RECURSE INPUT_KEYBOARD_SOURCES src/Input/Keyboard/*.cpp)
file(GLOB_RECURSE INPUT_MOUSE_SOURCES src/Input/Mouse/*.cpp)
file(GLOB_RECURSE INPUT_GAMEPAD_SOURCES src/Input/Gamepad/*.cpp)
# /Viewport
file(GLOB_RECURSE VIEWPORT_SOURCES src/Viewport/*.cpp)
# /Config
file(GLOB_RECURSE CONFIG_SOURCES src/Config/*.cpp)

add_executable(${PROJECT_NAME} src/main.cpp ${GLAD_SRC} ${UTILS_SOURCES} ${SHADER_SOURCES} ${INPUT_KEYBOARD_SOURCES} ${INPUT_MOUSE_SOURCES} ${INPUT_GAMEPAD_SOURCES} ${VIEWPORT_SOURCES} ${CONFIG_SOURCES})

# Set runtime library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")  # Use /MD for Release, /MDd for Debug

# Link against GLFW, OpenGL, and Assimp
target_link_libraries(${PROJECT_NAME} glfw3 opengl32 assimp-vc143-mt)
