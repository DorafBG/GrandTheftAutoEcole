#ifndef VOITURE_H
#define VOITURE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <GL/glew.h>

class Voiture : public Shape {
public:
    Voiture(Shader* shader_program, Texture* texture_bleu, Texture* texture_noir);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Voiture();

private:
    GLuint VAO;
    GLuint vbo;
    GLuint ebo_bleu;
    GLuint ebo_noir;
    Texture* texture_bleu;
    Texture* texture_noir;
    GLuint loc_diffuse_map;

    int num_indices_bleu;
    int num_indices_noir;
};

#endif