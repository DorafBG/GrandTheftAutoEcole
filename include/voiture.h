#ifndef VOITURE_H
#define VOITURE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <GL/glew.h>

class Voiture : public Shape {
public:
    Voiture(Shader* shader_program, Texture* texture);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Voiture();

private:
    GLuint VAO;
    GLuint buffers[2];
    Texture* texture;
    GLuint loc_diffuse_map;
};

#endif