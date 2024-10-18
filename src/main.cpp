// main.cpp
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
#include "GUI/ImGUI/ImGUI.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Initialize variables for frame rate calculation
// TODO: Place in a config
static double lastTime = glfwGetTime();
static int frameCount = 0;
static float frameRate = 0.0f;
float deltaTime = 0.0f;          // Time between current frame and last frame
float lastFrame = 0.0f;          // Time of the last frame

// struct Mesh {
//     std::vector<float> vertices;
//     std::vector<unsigned int> indices;
//     GLuint VAO, VBO, EBO;

//     Mesh(const std::vector<float>& verts, const std::vector<unsigned int>& inds)
//         : vertices(verts), indices(inds) {
//         setupMesh();
//     }

//     void setupMesh() {
//         glGenVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);
//         glGenBuffers(1, &EBO);

//         glBindVertexArray(VAO);

//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

//         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
//         glEnableVertexAttribArray(0);
//         // Add more vertex attributes as necessary (e.g., texture coords, normals)
        
//         glBindVertexArray(0);
//     }

//     void draw() {
//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//         glBindVertexArray(0);
//     }
// };

// Mesh loadModel(const std::string& path) {
//     Assimp::Importer importer;
//     const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
//     if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//         std::cerr << "ERROR: Assimp failed to load model: " << importer.GetErrorString() << std::endl;
//         return Mesh({}, {}); // Return empty mesh on failure
//     }

//     std::vector<float> vertices;
//     std::vector<unsigned int> indices;

//     for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
//         aiMesh* mesh = scene->mMeshes[i];
//         for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
//             aiVector3D pos = mesh->mVertices[j];
//             vertices.push_back(pos.x);
//             vertices.push_back(pos.y);
//             vertices.push_back(pos.z);
//             // Add more attributes as necessary (e.g., texture coords, normals)
//         }

//         for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
//             aiFace face = mesh->mFaces[j];
//             for (unsigned int k = 0; k < face.mNumIndices; k++) {
//                 indices.push_back(face.mIndices[k]);
//             }
//         }
//     }
    
//     return Mesh(vertices, indices);
// }


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

int initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "ERROR: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    return 0;
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
    glfwSwapInterval(vSyncEnabled); // V-Sync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    initGLAD();

    initImGUI(window, imGUIEnabled);

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

    // // Set up vertex data and buffers
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
    // Mesh model = loadModel("C:/Users/jayxw/Desktop/test/FLUX/models/mazda-rx-7/source/rx7.fbx");

    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
        processInput(window, cameraPos, currentCameraSpeed, static_cast<float>(deltaTime)); // Pass by reference                 // Process input

        frameCount++;
        double currentFrame = glfwGetTime();
        deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = static_cast<float>(currentFrame);
        if (currentFrame - lastTime >= 1.0)
        { // Update frame rate every second
            frameRate = frameCount / static_cast<float>((currentFrame - lastTime));
            lastTime = currentFrame;
            frameCount = 0;
        }

        dearImGuiBaby({cameraPos.x, cameraPos.y, cameraPos.z}, currentCameraSpeed, frameRate, cameraSpeed, deltaTime, imGUIEnabled); // Pass camera position
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        

        // Set the uniform for projection and view matrices
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // Identity model matrix

        // model.draw(); 

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the triangles
        renderImGUI(imGUIEnabled);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    cleanUpImGUI(imGUIEnabled);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}
