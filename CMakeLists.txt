cmake_minimum_required(VERSION 3.10)
project(FLUXEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_INCLUDE_DIR "C:/glfw/include")
set(GLFW_LIBRARY_DIR "C:/glfw/lib-vc2022")

set(GLAD_INCLUDE_DIR "C:/glad/include")

set(GLM_INCLUDE_DIR "C:/glm")

set(ASSIMP_INCLUDE_DIR "C:/Program Files/Assimp/include")
set(ASSIMP_LIBRARY_DIR "C:/Program Files/Assimp/lib/x64")

set(IMGUI_INCLUDE_DIR "C:/imgui")
set(IMGUI_SRC
    "C:/imgui/imgui.cpp"
    "C:/imgui/imgui_demo.cpp"
    "C:/imgui/imgui_draw.cpp"
    "C:/imgui/imgui_tables.cpp"
    "C:/imgui/imgui_widgets.cpp"
    "C:/imgui/imgui_impl_glfw.cpp"
    "C:/imgui/imgui_impl_opengl3.cpp"
)

include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${GLM_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} "src")

link_directories(${GLFW_LIBRARY_DIR} ${ASSIMP_LIBRARY_DIR})

set(GLAD_SRC "C:/glad/src/glad.c")

# /Utils
file(GLOB_RECURSE UTILS_SOURCES src/Utils/*.cpp)
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
# /GUI
file(GLOB_RECURSE GUI_SOURCES src/GUI/*.cpp)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${UTILS_SOURCES}
    ${SHADER_SOURCES}
    ${GLAD_SRC}
    ${INPUT_KEYBOARD_SOURCES}
    ${INPUT_MOUSE_SOURCES}
    ${INPUT_GAMEPAD_SOURCES}
    ${VIEWPORT_SOURCES}
    ${CONFIG_SOURCES}
    ${GUI_SOURCES}
    ${IMGUI_SRC}  # Include ImGui sources
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")  # Use /MD for Release, /MDd for Debug

target_link_libraries(${PROJECT_NAME} glfw3 opengl32 assimp-vc143-mt)
