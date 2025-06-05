#include "banc.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

Banc::Banc(Shader* shader, const glm::vec3& color)
    : Shape(shader), color_(color)
{
    // Pieds
    float hauteur_pied = 1.0f;
    float rayon_pied = 0.07f;

    // 4 pieds, positionnés aux coins
    float longueur_assise = 2.0f;
    float dx = longueur_assise / 2.0f - 0.15f;
    float dz = 0.25f;
    pieds_.push_back(new Cylinder(shader, hauteur_pied, rayon_pied, 16));
    pieds_.push_back(new Cylinder(shader, hauteur_pied, rayon_pied, 16));
    pieds_.push_back(new Cylinder(shader, hauteur_pied, rayon_pied, 16));
    pieds_.push_back(new Cylinder(shader, hauteur_pied, rayon_pied, 16));

    // Assise et dossier : couleur personnalisable
    assise_ = new CubeSimple(shader, glm::vec3(0.8f, 0.6f, 0.3f));   // couleur bois clair
    dossier_ = new CubeSimple(shader, glm::vec3(0.7f, 0.5f, 0.2f));  // couleur bois un peu plus foncé
}

Banc::~Banc() {
    for (auto pied : pieds_) delete pied;
    delete assise_;
    delete dossier_;
}

void Banc::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3f(colorLoc, color_.r, color_.g, color_.b);

    // Placement des 4 pieds
    float longueur_assise = 2.0f;
    float hauteur_pied = 1.0f;
    float dx = longueur_assise / 2.0f - 0.15f;
    float dz = 0.25f;
    glm::vec3 positions[4] = {
        glm::vec3(-dx, hauteur_pied/2, -dz),
        glm::vec3( dx, hauteur_pied/2, -dz),
        glm::vec3(-dx, hauteur_pied/2,  dz),
        glm::vec3( dx, hauteur_pied/2,  dz),
    };

    for (int i = 0; i < 4; ++i) {
        glm::mat4 pied_mat = glm::translate(model, positions[i]);
        pied_mat = glm::rotate(pied_mat,glm::radians(90.0f),glm::vec3(1,0,0));
        pieds_[i]->draw(pied_mat, view, projection);
    }

    // Assise (cube plat posé sur les pieds)
    {
        glm::mat4 assise_mat = glm::translate(model, glm::vec3(0, hauteur_pied + 0.08f, 0));
        assise_mat = glm::scale(assise_mat, glm::vec3(2.0f, 0.13f, 0.4f)); // long, plat, peu épais
        assise_->draw(assise_mat, view, projection);
    }

    // Dossier (cube vertical derrière l’assise)
    {
        glm::mat4 dossier_mat = glm::translate(model, glm::vec3(0, hauteur_pied + 0.32f, -0.24f));
        dossier_mat = glm::scale(dossier_mat, glm::vec3(2.0f, 0.38f, 0.13f)); // long, plus haut, fin en Z
        dossier_->draw(dossier_mat, view, projection);
    }
}
