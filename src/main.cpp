<<<<<<< Updated upstream
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
#include "Utils/Paths/ShaderPaths.h"
#include "Utils/FrameBufferSize/FrameBufferSizeCallback.h"
#include "Viewport/Camera/Camera.h"
#include "GUI/ImGUI/ImGUI.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

static double lastTime = glfwGetTime();
static int frameCount = 0;
static float frameRate = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

// Function to load GLTF model using Assimp
bool loadGLTF(const std::string &path, std::vector<float> &vertices, std::vector<unsigned int> &indices)
{
    std::cout << "Loading GLTF model: " << path << std::endl;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0]; // Assuming only one mesh for simplicity

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // Texture coordinates (assuming existence)
        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::cout << "Loaded GLTF model successfully: "
              << path << " with "
              << vertices.size() / 5 << " vertices and " // Each vertex consists of 5 floats (3 for position + 2 for texCoords)
              << indices.size() << " indices." << std::endl;

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
// Function to link compiled shaders into a program
GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
    // Create a shader program
    GLuint shaderProgram = glCreateProgram();

    // Attach vertex and fragment shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Link the shaders into a program
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED/n" << infoLog << std::endl;
        return 0;
    }

    // Cleanup: delete shaders as they are now linked into the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
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
    assert(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    if (!loadGLTF("C:/Users/jayxw/Desktop/test/FLUX/models/haunted_house/scene.gltf", vertices, indices))
    {
        std::cerr << "Failed to load GLTF file." << std::endl;
        return -1;
    }

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
    glfwSwapInterval(vSyncEnabled);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
=======
#include "Model/Model.h"


const unsigned int width = 1920;
const unsigned int height = 1080;


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);
>>>>>>> Stashed changes

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("C:/Users/jayxw/Desktop/test/FLUX/src/Shader/default.vert", "C:/Users/jayxw/Desktop/test/FLUX/src/Shader/default.frag");

<<<<<<< Updated upstream
    glEnable(GL_DEPTH_TEST);

    std::string vertexShaderSource = LoadShader(SHADER_DIR + "vertexShader.glsl");
    std::string fragmentShaderSource = LoadShader(SHADER_DIR + "fragmentShader.glsl");

    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    GLuint shaderProgram = LinkShaderProgram(vertexShader, fragmentShader);

    glUseProgram(shaderProgram);

    Mesh model(vertices, indices);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model.draw(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
=======
	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 0.0f));

	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string modelPath = "C:/Users/jayxw/Desktop/test/FLUX/models/sword/scene.gltf";
	
	// Load in a model
	Model model("C:/Users/jayxw/Desktop/test/FLUX/models/sword/scene.gltf");

	// Original code from the tutorial
	// Model model("models/bunny/scene.gltf");
	GLenum err = 0;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.27f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Draw a model
		model.Draw(shaderProgram, camera);

		while ((err == glGetError()) != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error: " << err << std::endl;
		}

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
>>>>>>> Stashed changes
}
