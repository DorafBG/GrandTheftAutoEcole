#include "arbre.h"
#include <glm/gtc/matrix_transform.hpp>

Arbre::Arbre(Shader* shader)
    : Shape(shader)
{
    // Tronc marron
    tronc_ = new Cylinder(shader, 1.2f, 0.18f, 24);

    // Feuillage vert foncé
    feuilles_ = new CubeSimple(shader, glm::vec3(0.13f, 0.4f, 0.14f));

    position_ = glm::vec3(0.0f);

}

Arbre::~Arbre() {
    delete tronc_;
    delete feuilles_;
}

void Arbre::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    // On part du modèle déjà translaté à la position de l’arbre
    glm::mat4 arbreModel = glm::translate(model, position_);

    // --- TRONC ---
    glUseProgram(this->shader_program_);
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3f(colorLoc, 0.42f, 0.23f, 0.07f); // marron

    glm::mat4 troncMat = glm::translate(arbreModel, glm::vec3(0, 0.6f, 0));
    troncMat = glm::rotate(troncMat, glm::radians(90.0f), glm::vec3(1, 0, 0));
    tronc_->draw(troncMat, view, projection);

    // --- FEUILLAGE ---
    glUniform3f(colorLoc, 0.13f, 0.4f, 0.14f); // vert foncé

    glm::mat4 feuillesMat = glm::translate(arbreModel, glm::vec3(0, 1.35f, 0));
    feuillesMat = glm::scale(feuillesMat, glm::vec3(0.9f, 1.0f, 0.9f));
    feuilles_->draw(feuillesMat, view, projection);
}

