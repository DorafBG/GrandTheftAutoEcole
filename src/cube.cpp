#include "cube.h"
#include <GL/glew.h>

Cube::Cube(Shader* shader_program, const glm::vec3& color)
    : Shape(shader_program), color_(color)
{
    // Cube vertex positions
    GLfloat vertex_buffer_data[] = {
        // Face bas (y = -0.5)
        -0.5f, -0.5f, -0.5f, // 0
         0.5f, -0.5f, -0.5f, // 1
         0.5f, -0.5f,  0.5f, // 2
        -0.5f, -0.5f,  0.5f, // 3
        // Face haut (y = +0.5)
        -0.5f,  0.5f, -0.5f, // 4
         0.5f,  0.5f, -0.5f, // 5
         0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    GLint indices[] = {
        // Face bas
        0, 1, 2,
        2, 3, 0,
        // Face haut
        4, 5, 6,
        6, 7, 4,
        // Face avant
        3, 2, 6,
        6, 7, 3,
        // Face arrière
        0, 1, 5,
        5, 4, 0,
        // Face gauche
        0, 3, 7,
        7, 4, 0,
        // Face droite
        1, 2, 6,
        6, 5, 1
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, buffers);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Cube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);

    // Envoie la couleur au shader
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_[0]);

    glBindVertexArray(VAO);

    // Envoie matrices
    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}

void Cube::key_handler(int key) {
    // Optionnel: gère le clavier si tu veux
}

void Cube::setColor(const glm::vec3& color) {
    color_ = color;
}
