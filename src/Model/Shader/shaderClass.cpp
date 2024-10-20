#include "shaderClass.h"

// Reads a text file and outputs a string with everything within the text file
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);

    if (!in)
    {
        throw std::runtime_error("ERROR: Could not open file: " + std::string(filename));
    }

    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::string contents(size, '\0');

    if (!in.read(&contents[0], size))
    {
        throw std::runtime_error("ERROR: Could not read file " + std::string(filename));
    }
    return contents;
}

// Constructor that builds the shader program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    // Read the vertexFile and fragmentFile and store the strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Convert the shader source strings into character arrays
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create the Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the Vertex Shader into machince code
    glCompileShader(vertexShader);
    // Check if the shader compiled successfully
    compileErrors(vertexShader, "VERTEX");

    // Create the Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the Fragment Shader Source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile the Fragment Shader into machince code
    glCompileShader(fragmentShader);
    // Check if the fragment compiled successfully
    compileErrors(fragmentShader, "FRAGMENT");

    // Create the Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the shader program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/link all the shaders together into the shader program
    glLinkProgram(ID);
    // Checks if the shaders are linked successfully
    compileErrors(ID, "PROGRAM");
    // Delete the now useless Vertex and Fragment shader objects
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
};

// Activates the shader program
void Shader::Activate()
{
    glUseProgram(ID);
}

// Deletes the shader program
void Shader::Delete()
{
    glDeleteProgram(ID);
}

// Checks if the shaders have compiled successfully
void Shader::compileErrors(unsigned int shader, const char *type)
{
    // Stores the status of the compilation
    GLint hasCompiled;
    // Character array to store the error message in
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "SHADER_COMPILATION_ERROR for: " << type << "\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "SHADER_LINKING_ERROR for: " << type << "\n"
                      << infoLog << std::endl;
        }
    }
}
