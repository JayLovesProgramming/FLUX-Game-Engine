// ShaderLoader.h
#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <glad/glad.h>  // Include OpenGL headers first
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Loads a shader from the specified file path
std::string LoadShader(const std::string& filePath);

#endif // SHADER_LOADER_H
