#include "panneau.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

Panneau::Panneau(Shader* shader_support, const glm::vec3& color_support, Shader* shader_cube, Texture* texture_panneau)
    : Shape(shader_support), color_support_(color_support)
{
    // Support (mât fin)
    support_ = new Cylinder(shader_support, 1.6f, 0.07f, 24);

    // Panneau (cube très fin et large)
    panneau_cube_ = new Cube(shader_cube, texture_panneau);
}

Panneau::~Panneau() {
    delete support_;
    delete panneau_cube_;
}

void Panneau::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    // support
    glUseProgram(support_->shader_program_);
    GLint colorLoc = glGetUniformLocation(support_->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_support_[0]);
    glm::mat4 supportMat = glm::translate(model, glm::vec3(0, 0.8f, 0));
    supportMat = glm::rotate(supportMat,glm::radians(90.0f),glm::vec3(1,0,0));
    support_->draw(supportMat, view, projection);

    // panneau
    glm::mat4 panneauMat = glm::translate(model, glm::vec3(0, 1.68f, 0));
    panneauMat = glm::scale(panneauMat, glm::vec3(0.08f, 0.7f , 0.7f)); 
    panneauMat = glm::rotate(panneauMat,glm::radians(90.0f),glm::vec3(0,0,1));
    panneau_cube_->draw(panneauMat, view, projection);
}
