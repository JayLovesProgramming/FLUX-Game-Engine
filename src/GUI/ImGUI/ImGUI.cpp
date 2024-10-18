// ImGUI.cpp
#include "ImGUI.h"
#include "C:\Users\jayxw\Desktop\test\FLUX\src\Config\Config.h"

constexpr const char *iniFileDir = "C:/Users/jayxw/Desktop/test/FLUX/src/GUI/ImGUI/imgui.ini";

void setCustomColors()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Set custom colors
    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);      // White text
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
    style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black border
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5f, 1.0f, 0.5f, 0.5f); // Blue title bar background
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.5f, 1.0f, 0.5f, 0.5f); // Darker blue when active
    
    // Baby green button colors
    style.Colors[ImGuiCol_Button] = ImVec4(0.5f, 1.0f, 0.5f, 0.5f);  // Baby green button
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.6f, 1.0f, 0.6f, 0.5f); // Lighter baby green when hovered
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.8f, 0.4f, 1.0f); // Darker baby green when active
}

void initImGUI(GLFWwindow *window, bool imGUIEnabled)
{
    if (imGUIEnabled)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = iniFileDir;
        (void)io;
         setCustomColors();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
}

void dearImGuiBaby(const std::array<float, 30> &cameraPos, float currentCameraSpeed, float frameRate, float cameraSpeed, float deltaTime, bool imGUIEnabled)
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
        if (ImGui::Button("Reset Camera Position"))
        {
            // cameraPos[0] = 0.0f;
            // cameraPos[1] = 0.0f;
            // cameraPos[2] = 0.0f;
        }
        ImGui::Text("Delta Time: %.7f", deltaTime);
        ImGui::End();
    }
}

void renderImGUI(bool imGUIEnabled)
{

    if (imGUIEnabled)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void cleanUpImGUI(bool imGUIEnabled)
{
    if (imGUIEnabled)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
