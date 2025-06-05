#include "maison.h"
#include <glm/gtc/matrix_transform.hpp>

Maison::Maison(Shader* shader_cube, Shader* shader_pyramid,/*const glm::vec3& color_cube*/ Texture* texture_mur, const glm::vec3& color_pyramid, float longueur, float largeur, float hauteur_mur, float hauteur_toit, int toit)
    : Shape(shader_cube), longueur_(longueur), largeur_(largeur), hauteur_mur_(hauteur_mur), hauteur_toit_(hauteur_toit), toit_(toit)
{
    cube_ = new Cube(shader_cube, texture_mur);
    pyramid_ = new Pyramid(shader_pyramid, color_pyramid, toit_);
}

Maison::~Maison() {
    delete cube_;
    delete pyramid_;
}

void Maison::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glm::mat4 scaled_model = glm::scale(model, glm::vec3(longueur_, hauteur_mur_, largeur_));
    cube_->draw(scaled_model/*model*/, view, projection);
    glm::mat4 pyramid_model = glm::translate(model, glm::vec3(0.0f, hauteur_mur_/2+hauteur_toit_/2, 0.0f));
    if (toit_ == 1 || toit_ == 2 || toit_ == 3) {
        pyramid_model = glm::scale(pyramid_model, glm::vec3(longueur_, hauteur_toit_, largeur_));
        pyramid_->draw(pyramid_model, view, projection);
    }
}