#ifndef CUBESIMPLE_H
#define CUBESIMPLE_H

#include "shape.h"
#include <glm/glm.hpp>

class CubeSimple : public Shape {
public:
    CubeSimple(Shader* shader_program, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));
    ~CubeSimple();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key);

    void setColor(const glm::vec3& color);

private:
    GLuint buffers[2];
    glm::vec3 color_;

protected :
    GLuint VAO;
};

#endif // CUBE_H
