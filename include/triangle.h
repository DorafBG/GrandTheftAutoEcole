#pragma once

#include "shape.h"
#include "shader.h"
#include <GL/glew.h>

class Triangle : public Shape {
public:
    Triangle(Shader *shader_program);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key);
    virtual ~Triangle();

private:
    GLuint VAO;
    GLuint VBO;
};