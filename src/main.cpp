#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "ground.h"
#include "voiture.h"
#include "cylinder.h"  
#include "eolienne.h"
#include "cube.h"
#include "maison.h"
#include "cagedefoot.h"
#include "traffic_light.h"
#include "cube_traffic_light.h"
#include <string>
#include <glm/gtx/string_cast.hpp> 
#include <GLFW/glfw3.h>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif
#include "../external/glfw/src/internal.h"
#ifndef TEXTURE_DIR
#error "TEXTURE_DIR not defined"
#endif

float cylinderRotation = 0.0f;
const float rotationSpeed = 20.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cylinderRotation = 20.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cylinderRotation = -20.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cylinderRotation = 0.0f;
    }
}

int main() {
    Viewer viewer;
    std::string shader_dir = SHADER_DIR;
    std::string texture_dir = TEXTURE_DIR;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // SOL
    Texture* grass_texture = new Texture(texture_dir + "tapis.jpg");
    Shader* ground_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    Shape* ground = new Ground(ground_shader, grass_texture);
    glm::mat4 ground_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f));
    Node* ground_node = new Node(ground_mat);
    ground_node->add(ground);
    viewer.scene_root->add(ground_node);

    // VOITURE
    glm::vec3 cam_pos = viewer.camera_pos;
    glm::vec3 cam_dir = glm::normalize(viewer.camera_front);
    float t = -cam_pos.y / cam_dir.y;
    glm::vec3 intersection_point = cam_pos + t * cam_dir;
    std::cout << "Camera regarde le sol en : " << glm::to_string(intersection_point) << std::endl;

    Shader* car_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    Texture* rouge_texture = new Texture(texture_dir + "95.png");
    Texture* noir_texture = new Texture(texture_dir + "noir.jpg");
    Voiture* voiture = new Voiture(texture_shader, rouge_texture, noir_texture);

    float largeur_voiture = 1.5f, epaisseur_voiture = 2.0f, longueur_voiture = 2.0f;
    glm::vec3 decalage_vertical = glm::vec3(0, 0.01f, 0);
    float rotation_voiture = -90.0f;

    glm::mat4 voiture_mat = glm::translate(glm::mat4(1.0f), intersection_point + decalage_vertical) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation_voiture), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(largeur_voiture, epaisseur_voiture, longueur_voiture));

    Node* voiture_node = new Node(voiture_mat);
    voiture_node->add(voiture);

    Shader* cylinder_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    float angle = viewer.angleRoues;
    glm::vec3 wheel_positions[4] = {
        {-0.5f, 0.16f,  0.6f},  // avant gauche
        { 0.5f, 0.16f, -0.65f}, // arrière droite
        { 0.5f, 0.16f,  0.6f},  // avant droite
        {-0.5f, 0.16f, -0.65f}  // arrière gauche
    };
    float wheel_rotations[4] = { -90.0f, 90.0f, 90.0f, -90.0f };

    for (int i = 0; i < 4; ++i) {
        Cylinder* cyl = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16);
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), wheel_positions[i]) *
            glm::rotate(glm::mat4(1.0f), glm::radians(wheel_rotations[i]), glm::vec3(0, 1, 0));
        Node* cyl_node = new Node(trans);
        cyl_node->add(cyl);
        voiture_node->add(cyl_node);
    }

    // Maisons
    Shader* shader_maison = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");

    Texture* mur1 = new Texture(texture_dir + "mur_marron.png");
    Maison* maison1 = new Maison(texture_shader, shader_maison, mur1, glm::vec3(0.2f, 0.08f, 0.0f), 0.8f, 1.0f, 0.5f, 0.4f, 1);
    glm::mat4 mat1 = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 1, 10)) * glm::scale(glm::mat4(1.0f), glm::vec3(4));
    Node* node1 = new Node(mat1); node1->add(maison1); viewer.scene_root->add(node1);

    Texture* mur2 = new Texture(texture_dir + "hlm.png");
    Maison* maison2 = new Maison(texture_shader, shader_maison, mur2, glm::vec3(0.0f), 1.5f, 1.5f, 3.0f, 0.0f, 0);
    glm::mat4 mat2 = glm::translate(glm::mat4(1.0f), glm::vec3(20, 6, 10)) * glm::scale(glm::mat4(1.0f), glm::vec3(4));
    Node* node2 = new Node(mat2); node2->add(maison2); viewer.scene_root->add(node2);

    Texture* mur3 = new Texture(texture_dir + "hlm.png");
    Maison* maison3 = new Maison(texture_shader, shader_maison, mur3, glm::vec3(0.0f), 0.5f, 5.0f, 5.0f, 0.0f, 0);
    glm::mat4 mat3 = glm::translate(glm::mat4(1.0f), glm::vec3(-30, 10, -10)) * glm::scale(glm::mat4(1.0f), glm::vec3(4));
    Node* node3 = new Node(mat3); node3->add(maison3); viewer.scene_root->add(node3);

    // Éoliennes
    Shader* shader_blanc = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
    Eolienne* e1 = new Eolienne(shader_blanc, glm::vec3(1.0f));
    glm::mat4 m1 = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0, 0));
    Node* n1 = new Node(m1); n1->add(e1); viewer.scene_root->add(n1); viewer.eoliennes.push_back(e1);

    Eolienne* e2 = new Eolienne(shader_blanc, glm::vec3(1.0f));
    glm::mat4 m2 = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 5));
    Node* n2 = new Node(m2); n2->add(e2); viewer.scene_root->add(n2); viewer.eoliennes.push_back(e2);

    // Cage de foot
    CageDeFoot* cage = new CageDeFoot(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_cage = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_cage = new Node(mat_cage); node_cage->add(cage); viewer.scene_root->add(node_cage);

    // Feu de circulation
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
    TrafficLight* traffic_light = new TrafficLight(color_shader);
    // Positionne le feu à l'endroit voulu (ex : x = -5, y = 0, z = 0)
    glm::mat4 mat_traffic_light = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
    // Crée un Node pour la transformation
    Node* node_traffic_light = new Node(mat_traffic_light);
    // Ajoute le feu au Node
    node_traffic_light->add(traffic_light);
    // Ajoute le Node à la racine de la scène
    viewer.scene_root->add(node_traffic_light);

    // --- AJOUTE CETTE LIGNE ---
    viewer.traffic_lights.push_back(traffic_light);

    // Ajout final de la voiture
    viewer.scene_root->add(voiture_node);
    viewer.voiture_node = voiture_node;

    viewer.run();
}
