#include "cylinder.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

Cylinder::Cylinder(Shader *shader_program, float height, float radius, int slices)
    : Shape(shader_program)
{
    // generate vertices
    std::vector<glm::vec3> vertices;
    for (int i = 0; i < slices; i++) {
        float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
        float x = radius * glm::cos(theta);
        float y = radius * glm::sin(theta);
        vertices.push_back(glm::vec3(x, y, 0.5f * height));
        vertices.push_back(glm::vec3(x, y, -0.5f * height));
    }

    // add top and bottom vertices
    vertices.push_back(glm::vec3(0.0f, 0.0f, 0.5f * height));
    vertices.push_back(glm::vec3(0.0f, 0.0f, -0.5f * height));

    // generate indices
    std::vector<unsigned int> indices;
    for (int i = 0; i < slices; i++) {
        indices.push_back(2 * i);
        indices.push_back(2 * i + 1);
        indices.push_back((2 * i + 2) % (2 * slices));
        indices.push_back(2 * i + 1);
        indices.push_back((2 * i + 3) % (2 * slices));
        indices.push_back((2 * i + 2) % (2 * slices));
        indices.push_back(2 * i);
        indices.push_back((2 * i + 2) % (2 * slices));
        indices.push_back(-2);
        indices.push_back(2 * i + 1);
        indices.push_back(-1);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    // create vertex buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // create index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    num_indices = static_cast<unsigned int>(indices.size());
}

void Cylinder::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{   
    glUseProgram(this->shader_program_);

    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);    

    glDrawElements(GL_TRIANGLE_STRIP, num_indices, GL_UNSIGNED_INT, nullptr);
}
