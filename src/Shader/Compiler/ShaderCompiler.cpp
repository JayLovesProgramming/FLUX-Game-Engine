// ShaderCompiler.cpp
#include "ShaderCompiler.h"

// TODO: Do more research on GLuint. Is it better to use GL values or unsigned int for example

GLuint CompileShader(const std::string& source, GLenum shaderType)
{
    // Create a shader object
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        std::cerr << "ERROR: Shader creation failed" << std::endl;
        return 0;
    }

    // Set the shader source code
    const char * src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check if the shader compilation was successful
    GLint isSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess)
    {
        // Get the length of the shader info log
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        // Retrieve amd print the info log
        std::string infoLog(logLength, ' ');
        glGetShaderInfoLog(shader, logLength, nullptr, &infoLog[0]);
        std::cerr << "ERROR: Shader compilation error: " << infoLog << std::endl;

        // Delete the shader since it failed to compile
        glDeleteShader(shader);
        return 0;
    }
    // Return the compiled shader if successful
    return shader;
}
