#include "Camera.h"

// Convert degrees to radians
float degreesToRadians(float degrees)
{
    return glm::radians(degrees);
}

// Calculate the front vector of the camera based on its yaw and pitch
glm::vec3 getCameraFront()
{
    glm::vec3 front;

    // Calculate the front vector components
    front.x = cos(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));
    front.y = sin(degreesToRadians(pitch));
    front.z = sin(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));

    // Normalize the front vector to ensure it's a unit vector
    return glm::normalize(front);
}
