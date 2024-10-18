// ControllerInput.cpp
// TODO: Make path relative
#include "ControllerInput.h"
#include "../../Viewport/Camera/Camera.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // GLM
#include <glm/gtc/type_ptr.hpp> // GLM

void processControllerInput(GLFWwindow *window, glm::vec3 &position, float cameraSpeed)
{
    // Check if a joystick is connected (assuming joystick 0)
    if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
    {
        const float deadZone = 0.2f; // Define a dead zone for the joystick
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, 0);
        const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, 0);

        // Use left joystick for movement (axes[0] for left-right, axes[1] for up-down)
        if (axes[1] > deadZone) // Move forward
        {
            position += getCameraFront() * cameraSpeed * axes[1];
        }
        if (axes[1] < -deadZone) // Move backward
        {
            position -= getCameraFront() * cameraSpeed * -axes[1];
        }
        if (axes[0] < -deadZone) // Move left
        {
            position -= glm::normalize(glm::cross(getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed * -axes[0];
        }
        if (axes[0] > deadZone) // Move right
        {
            position += glm::normalize(glm::cross(getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed * axes[0];
        }

        // Check for button presses (example: A button for some action)
        if (buttons[0] == GLFW_PRESS) // Replace 0 with the correct button index
        {
            // Perform some action, e.g., jump or shoot
        }
    }
}
