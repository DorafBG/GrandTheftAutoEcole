#ifndef MAISON_H
#define MAISON_H

#include "shape.h"
#include "cube.h"
#include "pyramid.h"
#include "texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Maison : public Shape {
public:
    Maison(Shader* shader_cube, Shader* shader_pyramid, Texture* texture_mur
        /*const glm::vec3& color_cube = glm::vec3(1.0f, 1.0f, 1.0f)*/,
        const glm::vec3& color_pyramid = glm::vec3(1.0f, 0.0f, 0.0f), 
        float longueur = 1.0f, float largeur = 1.0f, float hauteur_mur = 1.0f, float hauteur_toit = 1.0f, 
        int toit = 0);

    ~Maison();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cube* cube_;
    Pyramid* pyramid_;
    float longueur_, largeur_, hauteur_mur_, hauteur_toit_;
    int toit_;
};

#endif // MAISON_H

