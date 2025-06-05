#include "cubesimple.h"
#include <GL/glew.h>

CubeSimple::CubeSimple(Shader* shader_program, const glm::vec3& color)
    : Shape(shader_program), color_(color)
{
    // Cube vertex positions
    GLfloat vertex_buffer_data[] = {
    //  x,     y,     z,     nx,   ny,   nz

    // Face bas (y = -0.5, normale (0, -1, 0))
    -0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
     0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
     0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
    -0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,

    // Face haut (y = 0.5, normale (0, 1, 0))
    -0.5f, 0.5f, -0.5f,    0.f, 1.f, 0.f,
     0.5f, 0.5f, -0.5f,    0.f, 1.f, 0.f,
     0.5f, 0.5f,  0.5f,    0.f, 1.f, 0.f,
    -0.5f, 0.5f,  0.5f,    0.f, 1.f, 0.f,

    // Face avant (z = 0.5, normale (0, 0, 1))
    -0.5f, -0.5f, 0.5f,    0.f, 0.f, 1.f,
     0.5f, -0.5f, 0.5f,    0.f, 0.f, 1.f,
     0.5f,  0.5f, 0.5f,    0.f, 0.f, 1.f,
    -0.5f,  0.5f, 0.5f,    0.f, 0.f, 1.f,

    // Face arrière (z = -0.5, normale (0, 0, -1))
    -0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
     0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
     0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,
    -0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,

    // Face gauche (x = -0.5, normale (-1, 0, 0))
    -0.5f, -0.5f, -0.5f,  -1.f, 0.f, 0.f,
    -0.5f, -0.5f,  0.5f,  -1.f, 0.f, 0.f,
    -0.5f,  0.5f,  0.5f,  -1.f, 0.f, 0.f,
    -0.5f,  0.5f, -0.5f,  -1.f, 0.f, 0.f,

    // Face droite (x = 0.5, normale (1, 0, 0))
     0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,
     0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,
     0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,
     0.5f,  0.5f, -0.5f,   1.f, 0.f, 0.f,
};


    GLuint indices[] = {
    // Face bas
    0, 1, 2, 2, 3, 0,
    // Face haut
    4, 5, 6, 6, 7, 4,
    // Face avant
    8, 9,10,10,11, 8,
    // Face arrière
   12,13,14,14,15,12,
    // Face gauche
   16,17,18,18,19,16,
    // Face droite
   20,21,22,22,23,20,
};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, buffers);

    // Attribut 0 : position (3 floats)
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribut 1 : normale (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void CubeSimple::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
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

CubeSimple::~CubeSimple() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}

void CubeSimple::key_handler(int key) {
}

void CubeSimple::setColor(const glm::vec3& color) {
    color_ = color;
}
