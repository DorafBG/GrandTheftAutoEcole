#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include "shader.h"
#include "texture.h"
#include <GL/glew.h>
//#include <glm/glm.hpp>

class Cube : public Shape {
public:
    Cube(Shader* shader_program, /*const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)*/ Texture* texture_mur);
    //~Cube();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key);

    //void setColor(const glm::vec3& color);
    ~Cube();

private:
    GLuint VAO;
    GLuint vbo;
    GLuint ebo_mur;
    Texture* texture_mur;
    GLuint loc_diffuse_map;
    int num_indices_mur;
    //GLuint buffers[2];
    //glm::vec3 color_;
};

#endif // CUBE_H
