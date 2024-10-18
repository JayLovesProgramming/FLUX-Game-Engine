// ShaderCompiler.h
#pragma once

#include <string>
#include <glad/glad.h>
#include <iostream>

// ? Is there any benefit to using the GLAD int values?
GLuint CompileShader(const std::string& source, unsigned int shaderType);
