#include "cylinder.h"

#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <texture.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};


Cylinder::Cylinder(Shader *shader_program, float height, float radius, int slices)
    : Shape(shader_program)
{
    // generate vertices
    std::vector<Vertex> vertices;
    for (int i = 0; i < slices; i++) {
    float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
    float x = radius * glm::cos(theta);
    float y = radius * glm::sin(theta);
    glm::vec3 normal = glm::normalize(glm::vec3(x, y, 0.0f)); // normale radiale
    vertices.push_back({glm::vec3(x, y, 0.5f * height), normal});
    vertices.push_back({glm::vec3(x, y, -0.5f * height), normal});
}

    // Centre du capot supérieur
    vertices.push_back({glm::vec3(0.0f, 0.0f, 0.5f * height), glm::vec3(0, 0, 1)});
    // Centre du capot inférieur
    vertices.push_back({glm::vec3(0.0f, 0.0f, -0.5f * height), glm::vec3(0, 0, -1)});

    for (int i = 0; i < slices; i++) {
    float theta = 2.0f * glm::pi<float>() * static_cast<float>(i) / static_cast<float>(slices);
    float x = radius * glm::cos(theta);
    float y = radius * glm::sin(theta);
    // Bord du capot supérieur : normale (0, 0, 1)
    vertices.push_back({glm::vec3(x, y, 0.5f * height), glm::vec3(0, 0, 1)});
    // Bord du capot inférieur : normale (0, 0, -1)
    vertices.push_back({glm::vec3(x, y, -0.5f * height), glm::vec3(0, 0, -1)});
}

    // generate indices
    std::vector<unsigned int> indices;
    for (int i = 0; i < slices; ++i) {
    int next = (i + 1) % slices;
    int top1 = 2 * i;
    int bot1 = 2 * i + 1;
    int top2 = 2 * next;
    int bot2 = 2 * next + 1;

    indices.push_back(top1); indices.push_back(bot1); indices.push_back(top2);
    indices.push_back(bot1); indices.push_back(bot2); indices.push_back(top2);
    }

    // Capot supérieur (top fan)
    int top_center = 2 * slices;
    int top_edge_start = 2 * slices + 2;
    for (int i = 0; i < slices; ++i) {
        int curr = top_edge_start + 2 * i;
        int next = top_edge_start + (2 * ((i + 1) % slices));
        indices.push_back(top_center); indices.push_back(curr); indices.push_back(next);
    }

    // Capot inférieur (bottom fan)
    int bottom_center = 2 * slices + 1;
    int bottom_edge_start = 2 * slices + 3;
    for (int i = 0; i < slices; ++i) {
        int curr = bottom_edge_start + 2 * i;
        int next = bottom_edge_start + (2 * ((i + 1) % slices));
        indices.push_back(bottom_center); indices.push_back(next); indices.push_back(curr);
    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Attribut 0 : position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Attribut 1 : normale (vec3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

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

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);

}