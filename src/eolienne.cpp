#include "eolienne.h"
#include <glm/gtc/matrix_transform.hpp>

Eolienne::Eolienne(Shader* shader, const glm::vec3& color)
    : Shape(shader), color_(color)
{
    support_ = new Cylinder(shader, 5.0f, 0.2f, 32);   // mât
    axe_     = new Cylinder(shader, 0.8f, 0.1f, 16);   // axe horizontal
    for (int i = 0; i < 3; ++i){
        pales_.push_back(new Cylinder(shader, 1.5f, 0.07f, 16));
    }

}

Eolienne::~Eolienne() {
    delete support_;
    delete axe_;
    for (auto pale : pales_) delete pale;
}

void Eolienne::update(float deltaTime) {
    float vitesse = 60.0f; // degres/s
    angle_pales_ += vitesse * deltaTime;
    if (angle_pales_ > 360.0f) angle_pales_ -= 360.0f;
}



void Eolienne::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    // mat
    glm::mat4 support_mat = glm::translate(model, glm::vec3(0, 2.5f, 0));
    support_mat = glm::rotate(support_mat, glm::radians(90.0f), glm::vec3(1, 0, 0));
    support_->draw(support_mat, view, projection);

    // Axe horizontal (dans Z au sommet du mat)
    float rayon = 0.75f;
    float longueur_axe = rayon * 2.0f; // Pour aller de -rayon à +rayon sur Z
    glm::mat4 axe_mat = glm::translate(model, glm::vec3(0, 5.1f, 0));
    axe_mat = glm::rotate(axe_mat, glm::radians(90.0f), glm::vec3(0, 1, 0));
    axe_->draw(axe_mat, view, projection);

    for (int i = 0; i < 3; ++i) {
        float angle = glm::radians(120.0f * i) + glm::radians(angle_pales_);
        glm::mat4 pale_mat = glm::translate(model, glm::vec3(0, 5.0f, 0));
        pale_mat = glm::rotate(pale_mat, angle, glm::vec3(1, 0, 0)); // rotation autour de X
        pale_mat = glm::translate(pale_mat, glm::vec3(0.5, 0, rayon)); // placer la pale
        pales_[i]->draw(pale_mat, view, projection);
}

}     