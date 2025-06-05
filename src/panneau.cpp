#include "panneau.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

Panneau::Panneau(Shader* shader_support, const glm::vec3& color_support, Shader* shader_cylindre)
    : Shape(shader_support), color_support_(color_support)
{
    // Support (mât fin)
    support_ = new Cylinder(shader_support, 1.6f, 0.07f, 24);

    // Panneau (cylindre rouge, large et plat)
    cylindre_panneau_ = new Cylinder(shader_cylindre, 0.1f, 0.35f, 32); // hauteur faible, rayon large
}

Panneau::~Panneau() {
    delete support_;
    delete cylindre_panneau_;
}

void Panneau::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    // --- Dessin du support (gris ou couleur choisie)
    glUseProgram(support_->shader_program_);
    GLint colorLoc = glGetUniformLocation(support_->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_support_[0]);
    glm::mat4 supportMat = glm::translate(model, glm::vec3(0, 0.8f, 0));
    supportMat = glm::rotate(supportMat, glm::radians(90.0f), glm::vec3(1,0,0));
    support_->draw(supportMat, view, projection);

    // --- Dessin du panneau (cylindre rouge)
    glUseProgram(cylindre_panneau_->shader_program_);
    GLint colorLoc2 = glGetUniformLocation(cylindre_panneau_->shader_program_, "uColor");
    glm::vec3 rouge(1.0f, 0.0f, 0.0f);
    glUniform3fv(colorLoc2, 1, &rouge[0]);
    glm::mat4 panneauMat = glm::translate(model, glm::vec3(0, 1.68f, 0));
    panneauMat = glm::rotate(panneauMat, glm::radians(90.0f), glm::vec3(0,1,0));
    cylindre_panneau_->draw(panneauMat, view, projection);
}
