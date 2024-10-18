#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <array>
#include <glad/glad.h>                  // GLAD
#include <GLFW/glfw3.h>                 // GLFW
#include <glm/glm.hpp>                  // GLM
#include <glm/gtc/matrix_transform.hpp> // GLM
#include <glm/gtc/type_ptr.hpp>         // GLM

#include "Config/Config.h"
#include "Shader/Compiler/ShaderCompiler.h"
#include "Shader/Loader/ShaderLoader.h"
#include "Input/Mouse/MouseInput.h"
#include "Input/Keyboard/ProcessInput.h"
// #include "Input/Gamepad/ControllerInput.h"
#include "Utils/Paths/ShaderPaths.h"
#include "Utils/FrameBufferSize/FrameBufferSizeCallback.h"
#include "Viewport/Camera/Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Initialize variables for frame rate calculation
static double lastTime = glfwGetTime();
static int frameCount = 0;
static float frameRate = 0.0f;

// Vertex data (coordinates and texture coordinates)
const std::array<float, 30> vertices = {
    // Positions        // Texture Coords
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // Top-right
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   // Top-left
};

void cleanUpImGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void dearImGuiBaby(const std::array<float, 30> &cameraPos)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("FLUX System Properties");
    // Calculate frame rate
    frameCount++;
    double currentTime = glfwGetTime();
    if (currentTime - lastTime >= 1.0)
    { // Update frame rate every second
        frameRate = frameCount / static_cast<float>((currentTime - lastTime));
        lastTime = currentTime;
        frameCount = 0;
    }
    ImGui::Text("Frame Rate: %.2f FPS", frameRate); // Display frame rate
    // Display camera position, assuming cameraPos contains x, y, z at index 0, 1, 2
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", cameraPos[0], cameraPos[1], cameraPos[2]);
    ImGui::End(); // End the window
}

int main()
{
    // Init GLFW
    assert(glfwInit());

    // GLFW Window Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "FLUX Game Engine (Alpha Build 0.1)", monitor, nullptr);
    if (!window)
    {
        std::cerr << "ERROR: Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Disable V-Sync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Register mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "ERROR: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Load and compile shaders
    std::string vertexShaderSource = LoadShader(SHADER_DIR + "vertexShader.glsl");
    std::string fragmentShaderSource = LoadShader(SHADER_DIR + "fragmentShader.glsl");

    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // Main Render Loop
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Initial camera position

    while (!glfwWindowShouldClose(window))
    {

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
        processInput(window, cameraPos);                        // Process input
        dearImGuiBaby({cameraPos.x, cameraPos.y, cameraPos.z}); // Pass camera position
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Set the uniform for projection and view matrices
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // Identity model matrix

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the triangles

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    cleanUpImGUI();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
