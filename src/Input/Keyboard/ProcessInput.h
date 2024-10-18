// ProcessInput.h
#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern float cameraSpeed;
extern float currentCameraSpeed;

float processInput(GLFWwindow *window, glm::vec3 &position, float &currentCameraSpeed, float deltaTime);

