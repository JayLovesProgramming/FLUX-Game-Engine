cmake_minimum_required(VERSION 3.10)
project(YourProjectName VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the directory paths for GLFW
set(GLFW_INCLUDE_DIR "C:/glfw/include")
set(GLFW_LIBRARY_DIR "C:/glfw/lib-vc2022")

set(GLAD_INCLUDE_DIR "C:/glad/include")
set(GLM_INCLUDE_DIR "C:/glm")

# Include GLFW and GLAD header files
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

# Link GLFW libraries
link_directories(${GLFW_LIBRARY_DIR})


# Add your executable and include glad.c
add_executable(${PROJECT_NAME}  "C:/glad/src/glad.c" main.cpp)

# Set runtime library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")  # Use /MD for Release, /MDd for Debug

# Link against GLFW and OpenGL
target_link_libraries(${PROJECT_NAME} glfw3 opengl32)
