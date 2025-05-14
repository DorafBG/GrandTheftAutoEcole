#pragma once

#include "shape.h"
#include "texture.h"

class Ground : public Shape
{
public:
    Ground(Shader* shader_program, Texture* texture);
    ~Ground();
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    GLuint VAO, VBO, EBO;
    Texture* texture;
    GLint loc_diffuse_map;
};
