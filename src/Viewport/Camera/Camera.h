#pragma once
#include <glm/glm.hpp>

extern bool firstMouse; // Used to detect the first mouse movement
extern float lastX;     // Initial mouse X
extern float lastY;     // Initial mouse Y
extern float yaw;       // Yaw is initialized to this value
extern float pitch;     // Pitch is initialized to this value

glm::vec3 getCameraFront();
