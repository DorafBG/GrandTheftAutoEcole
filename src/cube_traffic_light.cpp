#include "cube_traffic_light.h"

CubeTrafficLight::CubeTrafficLight(Shader* shader, const glm::vec3& size)
    : CubeSimple(shader, size), dynamic_color_(glm::vec3(1.0f))
{}

void CubeTrafficLight::setColor(const glm::vec3& color) {
    dynamic_color_ = color;
}

void CubeTrafficLight::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);

    // Passe la couleur dynamique
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &dynamic_color_[0]);

    // Transmission des matrices
    Shape::draw(model, view, projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
