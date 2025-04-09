#include "voiture.h"
#include <iostream>
#include <winsock.h>
#include <ground.h>

Voiture::Voiture(Shader* shader_program, Texture* texture)
    : Shape(shader_program), texture(texture)
{
    // cube ecrase pr faire une voiture
    float vertices[] = {
        // positions           // UV coords
        // Front face
        -0.5f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  1.0f,   0.0f, 1.0f,

        // Back face
        -0.5f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -1.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, -1.0f,   1.0f, 1.0f,
    };

    unsigned int indices[] = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        4, 0, 3, 3, 7, 4,
        // Right
        1, 5, 6, 6, 2, 1,
        // Top
        3, 2, 6, 6, 7, 3,
        // Bottom
        4, 5, 1, 1, 0, 4
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Voiture::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());
    glUniform1i(loc_diffuse_map, 0);
    glBindVertexArray(VAO);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



Voiture::~Voiture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}

void Voiture::key_handler(int key) {
    return;
}
