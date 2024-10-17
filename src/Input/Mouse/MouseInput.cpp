#include "MouseInput.h"

bool firstMouse = true; // Used to detect the first mouse movement
float lastX = 0.0f;     // Initial mouse X
float lastY = 0.0f;     // Initial mouse Y
float yaw = -90.0f;     // Yaw is initialized to this value
float pitch = 0.0f;     // Pitch is initialized to this value

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false; // Do this only once
    }

    float xoffset = static_cast<float>(xpos) - lastX; // Calculate the offset
    float yoffset = lastY - static_cast<float>(ypos); // Invert y offset because the screen y-axis is inverted
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);
    
    const float sensitivity = 0.1f; // Adjust mouse sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset; // Update yaw
    pitch += yoffset; // Update pitch
    
    // Constrain pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}
