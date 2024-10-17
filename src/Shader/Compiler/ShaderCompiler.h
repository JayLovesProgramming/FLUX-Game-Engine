// ShaderCompiler.h
#pragma once

#include <string>
#include <glad/glad.h>

GLuint CompileShader(const std::string& source, GLenum shaderType);
