#ifndef CONTROLLERINPUT_H
#define CONTROLLERINPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Function declaration for processControllerInput
void processControllerInput(GLFWwindow* window, glm::vec3& position, float cameraSpeed);

#endif // CONTROLLERINPUT_H
