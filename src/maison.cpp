#include "maison.h"
#include <glm/gtc/matrix_transform.hpp>

Maison::Maison(Shader* shader_cube, Shader* shader_pyramid,
               const glm::vec3& color_cube, const glm::vec3& color_pyramid)
    : Shape(shader_cube)
{
    cube_ = new Cube(shader_cube, color_cube);
    pyramid_ = new Pyramid(shader_pyramid, color_pyramid);
}

Maison::~Maison() {
    delete cube_;
    delete pyramid_;
}

void Maison::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    cube_->draw(model, view, projection);
    glm::mat4 pyramid_model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
    pyramid_model = glm::scale(pyramid_model, glm::vec3(1.3f, 1.0f, 1.0f));
    pyramid_->draw(pyramid_model, view, projection);
}
