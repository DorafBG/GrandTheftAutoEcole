#pragma once

#include "shape.h"
#include "cylinder.h"
#include <glm/glm.hpp>

class CageDeFoot : public Shape {
public:
    CageDeFoot(Shader* shader, const glm::vec3& color = glm::vec3(1.0f));
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cylinder* poteau_gauche_;
    Cylinder* poteau_droit_;
    Cylinder* barre_transversale_;
    glm::vec3 color_;
};
