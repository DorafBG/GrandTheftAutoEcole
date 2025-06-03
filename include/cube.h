#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include <glm/glm.hpp>

class Cube : public Shape {
public:
    Cube(Shader* shader_program, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
    ~Cube();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key);

    void setColor(const glm::vec3& color);

private:
    GLuint VAO;
    GLuint buffers[2];
    glm::vec3 color_;
};

#endif // CUBE_H
