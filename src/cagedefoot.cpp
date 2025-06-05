#include "cagedefoot.h"

CageDeFoot::CageDeFoot(Shader* shader, const glm::vec3& color)
    : Shape(shader), color_(color)
{
    float hauteur = 2.0f;  // hauteur des poteaux
    float rayon = 0.07f;   // epaisseur cylindres
    float largeur = 2.44f; // largeur

    poteau_gauche_  = new Cylinder(shader, hauteur, rayon, 16);
    poteau_droit_   = new Cylinder(shader, hauteur, rayon, 16);
    barre_transversale_ = new Cylinder(shader, largeur, rayon, 16);
}

void CageDeFoot::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_[0]);

    // Poteau gauche (vertical sur Y)
    glm::mat4 mat_poteau_gauche = glm::translate(model, glm::vec3(-1.22f, 1.0f, 0.0f));
    mat_poteau_gauche = glm::rotate(mat_poteau_gauche, glm::radians(90.0f), glm::vec3(1, 0, 0));
    poteau_gauche_->draw(mat_poteau_gauche, view, projection);

    // Poteau droit (vertical sur Y)
    glm::mat4 mat_poteau_droit = glm::translate(model, glm::vec3(1.22f, 1.0f, 0.0f));
    mat_poteau_droit = glm::rotate(mat_poteau_droit, glm::radians(90.0f), glm::vec3(1, 0, 0));
    poteau_droit_->draw(mat_poteau_droit, view, projection);

    // Barre du haut (horizontale sur X au sommet sans derotation)
    glm::mat4 mat_barre = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
    mat_barre = glm::rotate(mat_barre, glm::radians(90.0f), glm::vec3(0, 1, 0));
    barre_transversale_->draw(mat_barre, view, projection);
}
