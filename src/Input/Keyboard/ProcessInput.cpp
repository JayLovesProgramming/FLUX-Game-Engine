// ProcessInput.cpp
#include "ProcessInput.h"
#include "../../Config/Config.h" // Include the header to access the global variables
#include "../Gamepad/ControllerInput.h"
#include "../../Viewport/Camera/Camera.h"

bool mouseEnabled = true;
bool imguiToggled = false;
float toggleCooldown = 0.5f;    // Half a second
float lastToggleTime = 0.0f;     // Time of the last toggle
float currentCameraSpeed = 0.01f; // Move this line here

float processInput(GLFWwindow *window, glm::vec3 &position, float &currentCameraSpeed, float deltaTime)
{
    // Update the currentCameraSpeed based on the defined camera speed in Config
     currentCameraSpeed = cameraSpeed * deltaTime; // Assuming cameraSpeed is defined in Config.h

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec3 front = getCameraFront();

    if (mouseEnabled)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            position += front * currentCameraSpeed; // Move forward
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            position -= front * currentCameraSpeed; // Move backward
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            position -= glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * currentCameraSpeed; // Move left
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            position += glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * currentCameraSpeed; // Move right
        }
    }

    // Get the current time
    float currentTime = static_cast<float>(glfwGetTime());

    // Check for Ctrl+I to toggle mouse input
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        if (!imguiToggled && (currentTime - lastToggleTime) >= toggleCooldown)
        {
            mouseEnabled = !mouseEnabled; // Toggle mouse enabled state
            imguiToggled = true;          // Set the flag to indicate that the combination is pressed
            lastToggleTime = currentTime; // Update the time of the last toggle
        }
    }

    // Reset the flag when either key is released
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
    {
        imguiToggled = false; // Reset the flag
    }

    // Set the cursor mode based on mouseEnabled
    glfwSetInputMode(window, GLFW_CURSOR, mouseEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    return currentCameraSpeed;
}
