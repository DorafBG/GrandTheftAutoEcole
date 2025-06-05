#include "voiture.h"
#include <iostream>
//#include <winsock.h>
#include <ground.h>

Voiture::Voiture(Shader* shader_program, Texture* texture_bleu, Texture* texture_noir)
    : Shape(shader_program), texture_bleu(texture_bleu), texture_noir(texture_noir)
{
    // lumiere
    light_dir_loc = glGetUniformLocation(shader_program_, "lightDir");
    light_color_loc = glGetUniformLocation(shader_program_, "lightColor");
    view_pos_loc = glGetUniformLocation(shader_program_, "viewPos");
    light_dir = glm::vec3(-1.0f, -1.0f, -1.0f);
    light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    view_pos = glm::vec3(0.0f, 0.0f, 5.0f);

    // cube ecrase pr faire une voiture
    float vertices[] = {
        // positions           // UV coords
        // c�t� droit        
        0.3f,  0.32f, -1.0f,  0.0f, 0.0f, //0
        0.5f,  0.32f, -0.9f,  0.0f, 0.0f, //1
        0.3f,  0.39f, -0.9f,  0.0f, 0.0f, //2
        0.5f,  0.39f,  1.0f,  0.0f, 0.0f, //3
        0.2f,  0.39f, -0.4f,  0.0f, 0.0f, //4
        0.4f,  0.39f, -0.3f,  0.0f, 0.0f, //5
        0.3f,  0.59f,  0.0f,  1.0f, 0.0f, //6
        0.3f,  0.39f,  0.9f,  1.0f, 1.0f, //7
        0.5f,  0.39f,  0.8f,  0.0f, 0.0f, //8
        0.5f,  0.55f,  0.8f,  0.0f, 0.0f, //9
        0.5f,  0.55f,  1.0f,  0.0f, 0.0f, //10
        0.3f,  0.05f, -1.0f,  0.0f, 0.0f, //11
        0.5f,  0.05f, -0.9f,  0.0f, 0.0f, //12
        0.5f,  0.05f,  0.75f, 0.0f, 0.0f, //13
        0.5f,  0.15f,  1.0f,  0.0f, 0.0f, //14
        0.5f,  0.39f, -0.8f,  0.0f, 0.0f, //15

        //c�t� gauche
       -0.3f,  0.32f, -1.0f,  0.0f, 0.0f, //16
       -0.5f,  0.32f, -0.9f,  0.0f, 0.0f, //17
       -0.3f,  0.39f, -0.9f,  0.0f, 0.0f, //18
       -0.5f,  0.39f,  1.0f,  0.0f, 0.0f, //19
       -0.2f,  0.39f, -0.4f,  0.0f, 0.0f, //20
       -0.4f,  0.39f, -0.3f,  0.0f, 0.0f, //21
       -0.3f,  0.59f,  0.0f,  0.0f, 0.0f, //22
       -0.3f,  0.39f,  0.9f,  0.0f, 1.0f, //23
       -0.5f,  0.39f,  0.8f,  0.0f, 0.0f, //24
       -0.5f,  0.55f,  0.8f,  0.0f, 0.0f, //25
       -0.5f,  0.55f,  1.0f,  0.0f, 0.0f, //26
       -0.3f,  0.05f, -1.0f,  0.0f, 0.0f, //27
       -0.5f,  0.05f, -0.9f,  0.0f, 0.0f, //28
       -0.5f,  0.05f,  0.75f, 0.0f, 0.0f, //29
       -0.5f,  0.15f,  1.0f,  0.0f, 0.0f, //30
       -0.5f,  0.39f, -0.8f,  0.0f, 0.0f, //31


        //x z y
    };

    /*unsigned int indices[] = {



    };*/

    unsigned int indices_bleu[] = {
        1, 12, 13, 1, 13, 14, 1, 14, 3, 1, 15, 3,
        0, 11, 12, 0, 1, 12, 


        17, 28, 29, 17, 29, 30, 17, 30, 19, 17, 31, 19,
        16, 27, 28, 16, 17, 28, 


        0, 16, 11, 16, 11, 27, 0, 2, 16, 16, 18, 2,
        13, 29, 14, 14, 30, 29, 3, 14, 19, 14, 19, 30,
        6, 7, 22, 7, 23, 22,
        2, 18, 3, 18, 3, 19, 2, 15, 3, 18, 19, 31
    };
    unsigned int indices_noir[] = {
        4, 5, 6, 5, 6, 7, 20, 21, 22, 21, 22, 23,
        4, 6, 20, 6, 22, 20,
        9, 10, 25, 10, 26, 25,
        8, 9, 10, 8, 10, 3, 24, 25, 26, 24, 26, 19,
        0, 2, 1, 2, 1, 15, 16, 18, 17, 18, 17, 31,


    };
    num_indices_bleu = sizeof(indices_bleu) / sizeof(unsigned int);
    num_indices_noir = sizeof(indices_noir) / sizeof(unsigned int);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /*glGenBuffers(2, &buffers[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
    glGenBuffers(1, &ebo_bleu);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_bleu);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_bleu), indices_bleu, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo_noir);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_noir);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_noir), indices_noir, GL_STATIC_DRAW);
}

void Voiture::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{

    glUseProgram(this->shader_program_);

    glBindVertexArray(VAO);
    loc_diffuse_map = glGetUniformLocation(shader_program_, "diffuse_map");

    glUniform3fv(light_dir_loc, 1, glm::value_ptr(light_dir));
    glUniform3fv(light_color_loc, 1, glm::value_ptr(light_color));
    glUniform3fv(view_pos_loc, 1, glm::value_ptr(view_pos));

    // Dessiner les triangles bleus
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_bleu->getGLid());
    glUniform1i(loc_diffuse_map, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_bleu);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, num_indices_bleu, GL_UNSIGNED_INT, 0);

    // Dessiner les triangles noirs
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_noir->getGLid());
    glUniform1i(loc_diffuse_map, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_noir);
    Shape::draw(model, view, projection);
    glDrawElements(GL_TRIANGLES, num_indices_noir, GL_UNSIGNED_INT, 0);
}



Voiture::~Voiture() {
    /*glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo_bleu);
    glDeleteBuffers(1, &ebo_noir);
}

void Voiture::key_handler(int key) {
    return;
}
