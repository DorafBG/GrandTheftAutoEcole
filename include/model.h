#ifndef MODEL_H
#define MODEL_H

#include "shape.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
    GLfloat position[3];
    GLfloat color[3];
};

class Model : public Shape {
public:
    Model(const std::string& path, Shader* shader);
    ~Model();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    bool loadModel(const std::string& path);
    void setupMesh();

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
};

#endif // MODEL_H
