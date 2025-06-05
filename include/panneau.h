#pragma once

#include "shape.h"
#include "cylinder.h"

class Panneau : public Shape {
public:
    Panneau(Shader* shader_support, const glm::vec3& color_support, Shader* shader_cylindre);
    ~Panneau();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cylinder* support_;
    Cylinder* cylindre_panneau_;
    glm::vec3 color_support_;
};