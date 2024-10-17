#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

extern float cameraSpeed; // Declare external variables for camera speed

void processInput(GLFWwindow *window, glm::vec3 &position); // Function declaration

#endif
