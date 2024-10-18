// ImGUI.h
#pragma once

#include <array>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void initImGUI(GLFWwindow *window);

void dearImGuiBaby(const std::array<float, 30> &cameraPos, float currentCameraSpeed, float frameRate, float cameraSpeed, float deltaTime);

void renderImGUI();

void cleanUpImGUI();
