#ifndef MAISON_H
#define MAISON_H

#include "shape.h"
#include "cube.h"
#include "pyramid.h"
#include <glm/glm.hpp>

class Maison : public Shape {
public:
    Maison(Shader* shader_cube, Shader* shader_pyramid,
           const glm::vec3& color_cube = glm::vec3(1.0f, 1.0f, 1.0f),
           const glm::vec3& color_pyramid = glm::vec3(1.0f, 0.0f, 0.0f));
    ~Maison();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cube* cube_;
    Pyramid* pyramid_;
};

#endif // MAISON_H
