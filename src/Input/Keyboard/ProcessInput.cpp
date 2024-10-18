// ProcessInput.cpp
#include "ProcessInput.h"

bool mouseEnabled = true;
bool imguiToggled = false;

float lastToggleTime = 0.0f;
float currentCameraSpeed = 0.01f;

const float toggleCooldown = 0.5f;    // Half a second
const glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f); // Global up direction

float processInput(GLFWwindow *window, glm::vec3 &position, float &currentCameraSpeed, float deltaTime)
{
    // Update the currentCameraSpeed based on the defined camera speed in Config
     currentCameraSpeed = updateCameraSpeed(deltaTime); // Assuming cameraSpeed is defined in Config.h

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glm::vec3 front = getCameraFront();

    if (mouseEnabled)
    {
        // Move Forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            position += front * currentCameraSpeed; 
        }
        // Move Backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            position -= front * currentCameraSpeed; 
        }
        // Move Left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            position -= glm::normalize(glm::cross(front, WORLD_UP)) * currentCameraSpeed;
        }
        // Move Right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            position += glm::normalize(glm::cross(front, WORLD_UP)) * currentCameraSpeed;
        }
    }

    // Get the current time
    float currentTime = static_cast<float>(glfwGetTime());
    handleMouseToggle(window, currentTime);
    
    // Set the cursor mode based on mouseEnabled
    glfwSetInputMode(window, GLFW_CURSOR, mouseEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

    return currentCameraSpeed;
}

void handleMouseToggle(GLFWwindow *window, float currentTime)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_I))
    {
        if (!imguiToggled)
        {
            mouseEnabled = !mouseEnabled;
            imguiToggled = true;
        }
    }

    // Reset toggle flag when keys are released
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE)
    {
        imguiToggled = false;
    }
}

// Update camera speed based on deltaTime
float updateCameraSpeed(float deltaTime)
{
    return cameraSpeed * deltaTime;
}
