#include "pyramid.h"

#include <iostream>

Pyramid::Pyramid(Shader *shader_program, const glm::vec3& color, int version) : Shape(shader_program), color_(color), version_(version)
{
    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLint> indices;

    if (version_ == 1) {
        vertex_buffer_data = {
            -0.6f, -0.5f, -0.6f,
             0.6f, -0.5f, -0.6f,
             0.6f, -0.5f,  0.6f,
            -0.6f, -0.5f,  0.6f,
             0.0f,  0.5f,  0.0f
        };

        indices = {
            0, 1, 2,
            2, 3, 0,
            0, 4, 1,
            1, 4, 2,
            2, 4, 3,
            3, 4, 0
        };
    }
    else if (version_ == 2) {
        vertex_buffer_data = {
            -0.5f, -0.5f, -0.6f,
             0.5f, -0.5f, -0.6f,
             0.5f, -0.5f,  0.6f,
            -0.5f, -0.5f,  0.6f,
            -0.5f,  0.5f,  0.0f,
             0.5f,  0.5f,  0.0f
        };

        indices = {
            0, 1, 2,
            2, 3, 0,
            0, 3, 4,
            1, 2, 5,
            0, 4, 5, 0, 1, 5,
            3, 4, 5, 3, 2, 5
        };
    }
    else if (version_ == 3) {
        vertex_buffer_data = {
            -0.6f, -0.5f, -0.5f,
            -0.6f, -0.5f,  0.5f,
             0.6f, -0.5f, -0.5f,
             0.6f, -0.5f,  0.5f,

             0.0f,  0.5f, -0.5f,
             0.0f,  0.5f,  0.5f
        };

        indices = {
            1, 5, 3,
            0, 4, 2,
            0, 1, 4, 1, 4, 5,
            5, 3, 2, 5, 4, 2,
        };
    }
    // --- OpenGL buffer setup ---
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, buffers); // crée 2 buffers

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(GLfloat), vertex_buffer_data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), indices.data(), GL_STATIC_DRAW);

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