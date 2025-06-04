#include "cube.h"
#include <GL/glew.h>

Cube::Cube(Shader* shader_program, /*const glm::vec3& color*/ Texture* texture_mur)
    : Shape(shader_program), /*color_(color)*/ texture_mur(texture_mur)
{
    // Cube vertex positions
    GLfloat vertex_buffer_data[] = {
        //positions // u v
        // Face avant (y = -0.5)
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,// 0
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,// 1
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,// 2
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f,// 3
         // Face arrière (y = +0.5)
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,// 4
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f,// 5
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,// 6
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f,// 7
    };

    GLint indices[] = {
        0, 1, 2, 1, 2, 3, //avant
        4, 5, 6, 5, 6, 7, //arrière
        0, 2, 4, 2, 4, 6, //gauche
        1, 3, 5, 3, 5, 7, //droite
        2, 3, 6, 3, 6, 7, //haut
    };

    num_indices_mur = sizeof(indices) / sizeof(unsigned int);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo_mur);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_mur);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*glGenBuffers(2, buffers);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);




    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
}

void Cube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);

    glBindVertexArray(VAO);
    loc_diffuse_map = glGetUniformLocation(shader_program_, "diffuse_map");

    // Dessiner les triangles bleus
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_mur->getGLid());
    glUniform1i(loc_diffuse_map, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_mur);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, num_indices_mur, GL_UNSIGNED_INT, 0);



    /*glUseProgram(this->shader_program_);

    // Envoie la couleur au shader
    GLint colorLoc = glGetUniformLocation(this->shader_program_, "uColor");
    glUniform3fv(colorLoc, 1, &color_[0]);

    glBindVertexArray(VAO);

    // Envoie matrices
    Shape::draw(model, view, projection);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo_mur);
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);*/
}

void Cube::key_handler(int key) {
    // Optionnel: gère le clavier si tu veux
}

/*void Cube::setColor(const glm::vec3& color) {
    color_ = color;
}
*/