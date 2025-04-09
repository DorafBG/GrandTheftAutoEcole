#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "ground.h"
#include "voiture.h"
#include <string>
#include <glm/gtx/string_cast.hpp> // pour afficher les vecteurs

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

#ifndef TEXTURE_DIR
#error "TEXTURE_DIR not defined"
#endif

int main()
{
    // create window, add shaders & scene objects, then run rendering loop
    Viewer viewer;

    // get shader and texture directories
    std::string shader_dir = SHADER_DIR;
    std::string texture_dir = TEXTURE_DIR;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // SOL
    Texture* grass_texture = new Texture(texture_dir + "tapis.jpg");
    Shader* ground_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    Shape* ground = new Ground(ground_shader, grass_texture);
    glm::mat4 ground_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f)); //50 fois plus large et profond (pas en hauteur)
    Node* ground_node = new Node(ground_mat);
    ground_node->add(ground);
    viewer.scene_root->add(ground_node);

    // VOITURE (rectangle posé là où la caméra regarde)
    glm::vec3 cam_pos = viewer.camera_pos;
    glm::vec3 cam_dir = glm::normalize(viewer.camera_front);

    float t = -cam_pos.y / cam_dir.y; // Intersection avec le plan y=0
    glm::vec3 intersection_point = cam_pos + t * cam_dir;

    std::cout << "Camera regarde le sol en : " << glm::to_string(intersection_point) << std::endl;

    Shader* car_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");
    Texture* car_texture = new Texture(texture_dir + "metal.jpg"); // texture pr la voiture

    Voiture* voiture = new Voiture(texture_shader, car_texture);

    // Dimensions et parametres
    float largeur_voiture = 1.5f;
    float epaisseur_voiture = 2.0f;
    float longueur_voiture = 2.0f;
    glm::vec3 decalage_vertical = glm::vec3(0, 0.01f, 0);
    float rotation_voiture = -90.0f; // rotation de -90degre autour de Y (si on met 90 la voiture est pas dans le bon sens)

    // Matrice de transformation
    glm::mat4 voiture_mat = glm::translate(glm::mat4(1.0f), intersection_point + decalage_vertical) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation_voiture), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(largeur_voiture, epaisseur_voiture, longueur_voiture));

    Node* voiture_node = new Node(voiture_mat);
    voiture_node->add(voiture);
    viewer.scene_root->add(voiture_node);
    viewer.voiture_node = voiture_node;  // permet de link la voiture au viewer (deplacement)

    // Lancer la boucle de rendu
    viewer.run();
}
