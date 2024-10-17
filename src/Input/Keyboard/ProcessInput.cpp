// ProcessInput.cpp
// TODO: Make relative path
#include "ProcessInput.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Input\Gamepad\ControllerInput.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Config\Config.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Viewport\Camera\Camera.h" // Include to use getCameraFront()

void processInput(GLFWwindow *window, glm::vec3 &position)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec3 front = getCameraFront();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += front * cameraSpeed; // Move forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= front * cameraSpeed; // Move backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed; // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed; // Move right
    }

    //  processControllerInput(window, position, cameraSpeed);
}
