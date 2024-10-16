#define MODELLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

// Do iostream instead?
#include <vector>
#include <string>

#include <glad/glad.h> 

class ModelLoader {
public:
    ModelLoader(const std::string& path);
    void Draw(unsigned int shaderProgram);
    
private:
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh *mesh, const aiScene *scene);

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
};
