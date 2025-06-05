#include "pyramid.h"

#include <iostream>


Pyramid::Pyramid(Shader *shader_program, const glm::vec3& color) : Shape(shader_program), color_(color)
{
    // Pyramid vertex positions
    GLfloat vertex_buffer_data[] = {
        // base
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        // top
         0.0f,  0.5f,  0.0f
    };

    // Pyramid indices
    GLint indices[] = {
        // base
        0, 1, 2,
        2, 3, 0,
        // sides
        0, 4, 1,
        1, 4, 2,
        2, 4, 3,
        3, 4, 0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Pyramid::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{   
    glUseProgram(this->shader_program_);

    // Envoie la couleur au shader
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_[0]);

    glBindVertexArray(VAO);
    
    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &buffers[0]);
}

void Pyramid::key_handler(int key) {
    return;
}
