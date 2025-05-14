#include "model.h"
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(const std::string& path, Shader* shader) : Shape(shader) {
    if (!loadModel(path)) {
        std::cerr << "Erreur lors du chargement du modèle : " << path << std::endl;
    }
    setupMesh();
}

Model::~Model() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Model::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string mtl_base_path = path.substr(0, path.find_last_of('/') + 1);
    
    // Chargement du fichier OBJ avec le chemin du matériau
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, 
                         path.c_str(), mtl_base_path.c_str())) {
        std::cerr << "Erreur TinyOBJLoader: " << err << std::endl;
        return false;
    }
    
    if (!warn.empty()) {
        std::cout << "TinyOBJ Warning: " << warn << std::endl;
    }

    std::cout << "Nombre de shapes: " << shapes.size() << std::endl;
    std::cout << "Nombre de matériaux: " << materials.size() << std::endl;
    
    // Parcours de tous les sommets
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            
            // Obtenir l'ID du matériau pour cette face
            int material_id = -1;
            if (shapes[s].mesh.material_ids.size() > 0) {
                material_id = shapes[s].mesh.material_ids[f];
            }
            
            // Parcours des sommets de la face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                Vertex vertex;
                
                // Position
                vertex.position[0] = attrib.vertices[3*idx.vertex_index+0];
                vertex.position[1] = attrib.vertices[3*idx.vertex_index+1];
                vertex.position[2] = attrib.vertices[3*idx.vertex_index+2];
                
                // Priorité pour les couleurs:
                // 1. Couleurs du vertex (si présentes dans le fichier)
                // 2. Couleur diffuse du matériau (si un matériau est assigné)
                // 3. Couleurs procédurales basées sur la position
                
                if (attrib.colors.size() > 0 && 3*idx.vertex_index+2 < attrib.colors.size()) {
                    // 1. Utiliser les couleurs du vertex si elles existent
                    vertex.color[0] = attrib.colors[3*idx.vertex_index+0];
                    vertex.color[1] = attrib.colors[3*idx.vertex_index+1];
                    vertex.color[2] = attrib.colors[3*idx.vertex_index+2];
                    std::cout << "Utilisation des couleurs du vertex" << std::endl;
                } 
                else if (material_id >= 0 && material_id < materials.size()) {
                    // 2. Utiliser la couleur diffuse du matériau
                    vertex.color[0] = materials[material_id].diffuse[0];
                    vertex.color[1] = materials[material_id].diffuse[1];
                    vertex.color[2] = materials[material_id].diffuse[2];
                    std::cout << "Utilisation du matériau " << material_id << ": " 
                              << vertex.color[0] << ", " << vertex.color[1] << ", " << vertex.color[2] << std::endl;
                } 
                else {
                    // 3. Générer une couleur procédurale basée sur la position
                    float normalizedX = fmod(abs(vertex.position[0]), 1.0f);
                    float normalizedY = fmod(abs(vertex.position[1]), 1.0f);
                    float normalizedZ = fmod(abs(vertex.position[2]), 1.0f);
                    
                    vertex.color[0] = 0.5f + 0.5f * sin(normalizedX * 6.28f);
                    vertex.color[1] = 0.5f + 0.5f * sin(normalizedY * 6.28f);
                    vertex.color[2] = 0.5f + 0.5f * sin(normalizedZ * 6.28f);
                    
                    // Alternative: couleur basée sur l'indice de la face pour des zones distinctes
                    // vertex.color[0] = 0.5f + 0.5f * sin(f * 0.1f);
                    // vertex.color[1] = 0.5f + 0.5f * sin(f * 0.1f + 2.0f);
                    // vertex.color[2] = 0.5f + 0.5f * sin(f * 0.1f + 4.0f);
                }
                
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
            }
            index_offset += fv;
        }
    }
    
    std::cout << "Modèle chargé avec " << vertices.size() << " sommets et " 
              << indices.size() << " indices" << std::endl;
    
    return true;
}

void Model::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Couleurs
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Model::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(shader_program_);

    GLuint modelLoc = glGetUniformLocation(shader_program_, "model");
    GLuint viewLoc = glGetUniformLocation(shader_program_, "view");
    GLuint projLoc = glGetUniformLocation(shader_program_, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}