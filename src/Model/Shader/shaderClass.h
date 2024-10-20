#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <stdexcept>
#include <system_error>

std::string get_file_contents(const char *fileName);

class Shader
{
public:
    // Reference ID of the shader program
    GLuint ID;
    // Constructor that builds the shader program from 2 different shaders
    Shader(const char *vertexFile, const char *fragmentFile);

    // Activates the shader program
    void Activate();
    // Deletes the shader program
    void Delete();

private:
    // Checks if the different shaders have compiled already
    void compileErrors(unsigned int shader, const char *type);
};
