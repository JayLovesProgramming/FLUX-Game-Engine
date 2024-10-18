// ShaderLoader.cpp
// TODO: Make path relative
#include <C:/Users/jayxw/Desktop/test/FLUX/src/Shader/Loader/ShaderLoader.h>

std::string LoadShader(const std::string& filePath)
{   
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "ERROR: Could not open shader file: "<< filePath << std::endl;
        return "";
    }

    // Read the file into a string
    std::string shaderCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // Close the file explicity (it will close on destruction but let's just ensure it closes)
    file.close();

    return shaderCode;
}
