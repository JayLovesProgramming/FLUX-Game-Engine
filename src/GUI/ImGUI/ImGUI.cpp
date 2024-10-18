// ImGUI.cpp
#include "ImGUI.h"

const bool imGUIEnabled = true;

void initImGUI(GLFWwindow *window)
{
    if (imGUIEnabled)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
}

void dearImGuiBaby(const std::array<float, 30> &cameraPos, float currentCameraSpeed, float frameRate, float cameraSpeed, float deltaTime)
{
    if (imGUIEnabled)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("FLUX System Properties");
        ImGui::Text("Performance Metrics");
        ImGui::Separator();
        ImGui::Text("Frame Rate: %.2f FPS", frameRate);
        ImGui::Spacing();
        ImGui::Text("Camera Properties");
        ImGui::Separator();
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPos[0], cameraPos[1], cameraPos[2]);
        ImGui::Text("Camera Speed: %.3f", cameraSpeed);
        ImGui::Text("Movement Speed: %.7f", currentCameraSpeed);
        ImGui::Spacing();
        ImGui::Text("Timing Metrics");
        ImGui::Separator();
        ImGui::Text("Delta Time: %.7f", deltaTime);
        ImGui::End();
    }
}

void renderImGUI()
{
    if (imGUIEnabled)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void cleanUpImGUI()
{
    if (imGUIEnabled)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
