// ShaderCompiler.cpp
#include "ShaderCompiler.h"
#include <iostream>

GLuint CompileShader(const std::string& source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint isSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR: Shader compilation error: " << infoLog << std::endl;
    }
    return shader;
}
