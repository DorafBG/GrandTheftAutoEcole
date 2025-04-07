// shape.cpp

#include "shape.h"

Shape::Shape(Shader *shader_program) : shader_program_(shader_program->get_id()) {
    
}

void Shape::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    
    GLint loc = glGetUniformLocation(this->shader_program_, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
    
    loc = glGetUniformLocation(this->shader_program_, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

    loc = glGetUniformLocation(this->shader_program_, "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
}