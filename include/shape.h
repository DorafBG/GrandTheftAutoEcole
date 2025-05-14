#pragma once

#include "shader.h"
#include "node.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

class Shape {
public:
    Shape(Shader *shader_program);

    virtual void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

protected:
    GLuint shader_program_;
};
