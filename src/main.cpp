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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Initialize variables for frame rate calculation
static double lastTime = glfwGetTime();
static int frameCount = 0;
static float frameRate = 0.0f;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of the last frame

struct Mesh
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;

    Mesh(const std::vector<float> &verts, const std::vector<unsigned int> &inds)
        : vertices(verts), indices(inds)
    {
        setupMesh();
    }

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Texture Coord attribute (if you use it)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void draw(GLuint shaderProgram)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// Function to load an OBJ file and its associated MTL file
bool loadOBJ(const std::string &path, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::string &mtlFileName)
{
    std::cout << "Loading object: " << path << std::endl;
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << path << std::endl;
        return false;
    }

    std::vector<float> temp_vertices;
    std::vector<unsigned int> temp_indices;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream s(line);
        std::string prefix;
        s >> prefix;

        if (prefix == "v") // Vertex
        {
            float x, y, z;
            s >> x >> y >> z;
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
        }
        else if (prefix == "f") // Face
        {
            unsigned int index;
            s >> index; // OBJ indices are 1-based, convert to 0-based
            temp_indices.push_back(index - 1);
        }
        else if (prefix == "mtllib") // Material file
        {
            s >> mtlFileName; // Read the material file name
        }
    }

    // Move data into the final vectors
    vertices = std::move(temp_vertices);
    indices = std::move(temp_indices);

    std::cout << "Loaded object successfully: "
              << path << " with "
              << vertices.size() / 5 << " vertices and " // Each vertex consists of 5 floats (3 for position + 2 for texCoords)
              << indices.size() << " indices." << std::endl;

    return true;
}
bool loadTexture(const std::string &filePath)
{
    int width, height, channels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return true;
}
// Function to load materials from the .mtl file
bool loadMTL(std::vector<GLuint> &textures)
{
    std::string path = "C:/Users/jayxw/Desktop/test/FLUX/models/bayard-station-valve-house/source/model/model.mtl";
    std::cout << "Loading material: " << path << std::endl;

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << path << std::endl;
        return false;
    }

    std::string line;
    std::string currentTextureName;

    while (std::getline(file, line))
    {
        std::istringstream s(line);
        std::string prefix;
        s >> prefix;

        if (prefix == "newmtl") // New material
        {
            s >> currentTextureName;
        }
        else if (prefix == "map_Kd") // Diffuse texture map
        {
            std::string texturePath;
            s >> texturePath;

            // unsigned int textureID;

            // GLenum error = glGetError();
            // if (error != GL_NO_ERROR)
            // {
            //     std::cerr << "OpenGL Error: " << error << std::endl;
            // }

            // glGenTextures(1, &textureID);
            // // glBindTexture(GL_TEXTURE_3D, textureID);

            // // // Load the texture using the helper function
            // if (!loadTexture(texturePath, textureID))
            // {
            //     return false; // Exit if loading failed
            // }

            // textures.push_back(textureID); // Store the texture ID for use
            std::cout << "Loaded texture: " << texturePath << " for material: " << currentTextureName << std::endl;
        }
    }

    return true;
}

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
    // Initialize GLFW
    assert(glfwInit());

    // Set GLFW Window Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Load the model and textures
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::string mtlFileName;

    if (!loadOBJ("C:/Users/jayxw/Desktop/test/FLUX/models/bayard-station-valve-house/source/model/model.obj", vertices, indices, mtlFileName))
    {
        std::cerr << "Failed to load OBJ file." << std::endl;
        return -1;
    }

    std::vector<unsigned int> textures; // For storing texture IDs
    if (!loadMTL(textures))       // Load materials from the associated .mtl file
    {
        std::cerr << "Failed to load MTL file." << std::endl;
        return -1;
    }

    // Now create the GLFW window
    GLFWmonitor *monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "FLUX Game Engine (Alpha Build 0.1)", monitor, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        const char *description;
        glfwGetError(&description);
        std::cerr << "GLFW Error: " << description << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    initGLAD();
    glfwSwapInterval(vSyncEnabled); // V-Sync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    Mesh mesh(vertices, indices);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Initial camera position

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        // processInput(window);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Set the view and projection matrices (update this according to your camera)
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        // Set uniforms
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Draw the mesh
        mesh.draw(shaderProgram);

        processInput(window, cameraPos, currentCameraSpeed, static_cast<float>(deltaTime));

        frameCount++;
        double currentFrame = glfwGetTime();
        deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = static_cast<float>(currentFrame);
        if (currentFrame - lastTime >= 1.0)
        {
            frameRate = frameCount / static_cast<float>((currentFrame - lastTime));
            lastTime = currentFrame;
            frameCount = 0;
        }

        dearImGuiBaby({cameraPos.x, cameraPos.y, cameraPos.z}, currentCameraSpeed, frameRate, cameraSpeed, deltaTime, imGUIEnabled);
        renderImGUI(imGUIEnabled);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUpImGUI(imGUIEnabled);
    // Cleanup resources
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
