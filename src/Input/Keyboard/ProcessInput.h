// ProcessInput.h
#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../../Config/Config.h" // Include the header to access the global variables
#include "../Gamepad/ControllerInput.h"
#include "../../Viewport/Camera/Camera.h"

extern float cameraSpeed;
extern float currentCameraSpeed;

float processInput(GLFWwindow *window, glm::vec3 &position, float &currentCameraSpeed, float deltaTime);

