// ShaderLoader.h
#pragma once
#include <glad/glad.h>  // Include OpenGL headers first
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Loads a shader from the specified file path
std::string LoadShader(const std::string& filePath);

