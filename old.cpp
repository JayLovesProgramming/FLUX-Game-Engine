#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex shader source code
const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos; // Position input from vertex buffer
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); // Output position to the clip space
}
)";

// Fragment shader source code
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor; // Output color
void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Set color to orange
}
)";

float lastX = 1024.0f / 2.0f; // Initial mouse position
float lastY = 768.0f / 2.0f;
bool firstMouse = true; // Used to detect the first mouse movement

// Camera attributes
float yaw = -90.0f; // Yaw is initialized to -90.0 degrees
float pitch = 0.0f; // Pitch is initialized to 0.0 degrees
float cameraSpeed = 0.05f; // Speed of camera movement

bool fullscreen = true;

// Mouse movement callback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false; // Do this only once
    }

    float xoffset = static_cast<float>(xpos) - lastX; // Calculate the offset
    float yoffset = lastY - static_cast<float>(ypos); // Invert y offset because the screen y-axis is inverted

    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    const float sensitivity = 0.1f; // Adjust mouse sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; // Update yaw
    pitch += yoffset; // Update pitch

    // Constrain pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// Function to calculate camera position based on yaw and pitch
glm::vec3 getCameraFront()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}

// Processes the keyboard input
void processInput(GLFWwindow *window, glm::vec3 &position)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Update the camera position based on keyboard input
    glm::vec3 front = getCameraFront();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        position += front * cameraSpeed; // Move forward
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        position -= front * cameraSpeed; // Move backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        position -= glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed; // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        position += glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed; // Move right
    }
}

// Function to compile shaders and create a shader program
unsigned int compileShaders()
{
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main()
{
    // Init GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl;
        return -1;
    }

    // GLFW Window Hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Determine if fullscreen or not
    GLFWmonitor *monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    // Create a window
    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Simple Window", monitor, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Compile shaders and create shader program
    unsigned int shaderProgram = compileShaders();

    // Define the vertices for a 3D triangle (with depth)
    float vertices[] = {
        // Positions
         0.0f,  0.5f, 0.0f,   // Top vertex
        -0.5f, -0.5f, 0.5f,   // Bottom left vertex
         0.5f, -0.5f, 0.5f,   // Bottom right vertex
         0.0f,  0.5f, 0.0f,   // Top vertex
         0.5f, -0.5f, 0.5f,   // Bottom right vertex
         0.0f, -0.5f, -0.5f   // Bottom right back vertex
    };

    // Create a Vertex Array Object (VAO)
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the VAO first, then bind and set the VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Setup projection and view matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    glm::mat4 view;
    glm::vec3 cameraPos(0.0f, 0.0f, 3.0f); // Initial camera position

    // Set the mouse input callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window, cameraPos);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate the shader program
        glUseProgram(shaderProgram);

        // Update the view matrix based on camera position
        view = glm::lookAt(cameraPos, cameraPos + getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));

        // Set the uniform matrices
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // Identity model matrix

        // Draw the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices (2 triangles)
        glBindVertexArray(0);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}
