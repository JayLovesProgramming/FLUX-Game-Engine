#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern float yaw;   // Declare external variables for yaw and pitch
extern float pitch;

glm::vec3 getCameraFront(); // Function declaration

#endif
