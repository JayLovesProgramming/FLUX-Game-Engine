// ShaderLoader.cpp
// TODO: Make path relative
#include <C:/Users/jayxw/Desktop/test/FLUX/src/Shader/Loader/ShaderLoader.h>

std::string LoadShader(const std::string& filePath)
{   
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "ERROR: Could not opne shader file: "<< filePath << std::endl;
        return "";
    }
    // Read the file into a string
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// !OLD
// std::string LoadShader(const std::string& filePath)
// {
//     std::ifstream file(filePath);
//     std::stringstream buffer;
//     if (file.is_open())
//     {
//         buffer << file.rdbuf();
//         file.close();
//     } else {
//         std::cerr << "ERROR: Could not open shader file: " << filePath << std::endl;
//     }
//     return buffer.str();
// }
