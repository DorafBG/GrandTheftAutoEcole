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
    float vitesse = 60.0f; // degrés par seconde (ajuste à ta convenance)
    angle_pales_ += vitesse * deltaTime;
    if (angle_pales_ > 360.0f) angle_pales_ -= 360.0f;
}



void Eolienne::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    // Support (mât)
    glm::mat4 support_mat = glm::translate(model, glm::vec3(0, 2.5f, 0));
    support_mat = glm::rotate(support_mat, glm::radians(90.0f), glm::vec3(1, 0, 0));
    support_->draw(support_mat, view, projection);

    // Axe horizontal (dans Z, au sommet du mât)
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



/*
void Eolienne::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    // 1. Support vertical (posé au sol, centré en (0, 2.5, 0))
    glm::mat4 support_mat = glm::translate(model, glm::vec3(0,2.5f, 1.7f));
    support_mat = glm::rotate(support_mat, glm::radians(90.0f), glm::vec3(1, 0, 0)); // Met le cylindre du Z vers le Y
    support_->draw(support_mat, view, projection);


    // 2. Axe horizontal (positionné au sommet du support, perpendiculaire à Y, donc rotation autour de Z de 90°)
    glm::mat4 axe_mat = glm::translate(model, glm::vec3(0, 5.0f, 1.7f)); // sommet du mât
    axe_mat = glm::rotate(axe_mat, glm::radians(90.0f), glm::vec3(1, 0, 0)); // rotation de 90° autour de X
    axe_->draw(axe_mat, view, projection);

    // 3. Pales : placées à l’extrémité de l’axe, puis chacune tournée de 0, 120, 240°
    for (int i = 0; i < 3; ++i) {
    float angle = glm::radians(120.0f * i);
    glm::mat4 pale_mat = axe_mat;
    pale_mat = glm::rotate(pale_mat, angle, glm::vec3(0, 1, 0));         // Répartir à 120°
    pale_mat = glm::translate(pale_mat, glm::vec3(0, 2.0f, 0.75f));          // Décaler en Y
    pales_[i]->draw(pale_mat, view, projection);
}


}*/


//pale_mat = glm::translate(pale_mat, glm::vec3(0, 2.0f, 0.75f));          