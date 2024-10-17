// Camera.h
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern float yaw;
extern float pitch;

glm::vec3 getCameraFront();
