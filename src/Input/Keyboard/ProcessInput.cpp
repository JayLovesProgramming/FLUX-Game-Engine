// ProcessInput.cpp
// TODO: Make relative path
#include "ProcessInput.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Input\Gamepad\ControllerInput.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Config\Config.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Viewport\Camera\Camera.h" 

bool mouseEnabled = true;                                              

void processInput(GLFWwindow *window, glm::vec3 &position)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec3 front = getCameraFront();
    if (mouseEnabled)
    {
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
    }

    // Check for Ctrl+I to toggle mouse input
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        mouseEnabled = !mouseEnabled; // Toggle mouse enabled state
    }
    glfwSetInputMode(window, GLFW_CURSOR, mouseEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    //  processControllerInput(window, position, cameraSpeed);
}
