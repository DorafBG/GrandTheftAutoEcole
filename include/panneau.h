#ifndef PANNEAU_H
#define PANNEAU_H

#include "shape.h"
#include "cylinder.h"
#include "cube.h"
#include <glm/glm.hpp>

class Panneau : public Shape {
public:
    // On passe le shader pour le support et la texture/couleur pour le cube
    Panneau(Shader* shader_support, const glm::vec3& color_support, Shader* shader_cube, Texture* texture_panneau);
    ~Panneau();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cylinder* support_;
    Cube* panneau_cube_;
    glm::vec3 color_support_;
};

#endif // PANNEAU_H
