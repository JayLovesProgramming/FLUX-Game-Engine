#include <C:\Users\jayxw\Desktop\test\glfw_concept\src\Shader\Loader\ShaderLoader.h>

std::string LoadShader(const std::string& filePath)
{
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (file.is_open())
    {
        buffer << file.rdbuf();
        file.close();
    } else {
        std::cerr << "ERROR: Could not open shader file: " << filePath << std::endl;
    }
    return buffer.str();
}
