// ShaderCompiler.h
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

GLuint CompileShader(const std::string& source, GLenum shaderType);

#endif // SHADER_H
