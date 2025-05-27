#include "viewer.h"
#include "textured_sphere.h"
#include "lighting_sphere.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "ground.h"
#include "voiture.h"
#include "cylinder.h"  
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

// Variables globales pour la rotation
float cylinderRotation = 0.0f;
const float rotationSpeed = 20.0f; // Degrés par seconde

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cylinderRotation = 20.0f; // Rotation fixe à 20°
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cylinderRotation = -20.0f; // Rotation fixe à -20°
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { // Reset avec R
        cylinderRotation = 0.0f;
    }
}

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
    Texture* rouge_texture = new Texture(texture_dir + "95.png");
    Texture* noir_texture = new Texture(texture_dir + "noir.jpg");
    Voiture* voiture = new Voiture(texture_shader, rouge_texture, noir_texture);

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

    //les roues sont des cylindres
    // Créer un shader pour le cylindre
    Shader* cylinder_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");



    /// PREMIER CYLINDRE
    Cylinder* cylinder = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // shader, hauteur=2, rayon=0.5, 16 segments

    // Créer un nœud pour le cylindre avec une transformation relative à la voiture
    glm::mat4 cylinder_transform = glm::mat4(1.0f);
    // Positionner le cylindre au-dessus de la voiture
    cylinder_transform = glm::translate(cylinder_transform, glm::vec3(-0.5f, 0.16f, 0.6f)); // 1.5 unités au-dessus
    cylinder_transform = glm::rotate(cylinder_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder_node = new Node(cylinder_transform);
    cylinder_node->add(cylinder);

    voiture_node->add(cylinder_node);

    float angle = viewer.angleRoues;


    // DEUXIÈME CYLINDRE
    Cylinder* cylinder2 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Créer un nœud pour le deuxième cylindre
    glm::mat4 cylinder2_transform = glm::mat4(1.0f);
    // Positionner le deuxième cylindre plus haut (au-dessus du premier)
    cylinder2_transform = glm::translate(cylinder2_transform, glm::vec3(0.5f, 0.16f, -0.65f)); // 3.5 unités au-dessus de la voiture
    cylinder2_transform = glm::rotate(cylinder2_transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Node* cylinder2_node = new Node(cylinder2_transform);
    cylinder2_node->add(cylinder2);

    // Ajouter le deuxième cylindre comme enfant du nœud voiture
    voiture_node->add(cylinder2_node);



    // 3e CYLINDRE
    Cylinder* cylinder3 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Créer un nœud pour le deuxième cylindre
    glm::mat4 cylinder3_transform = glm::mat4(1.0f);
    // Positionner le deuxième cylindre plus haut (au-dessus du premier)
    cylinder3_transform = glm::translate(cylinder3_transform, glm::vec3(0.5f, 0.16f, 0.6f)); // 3.5 unités au-dessus de la voiture
    cylinder3_transform = glm::rotate(cylinder3_transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder3_node = new Node(cylinder3_transform);
    cylinder3_node->add(cylinder3);

    // Ajouter le deuxième cylindre comme enfant du nœud voiture
    voiture_node->add(cylinder3_node);


    // 4e CYLINDRE
    Cylinder* cylinder4 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Créer un nœud pour le deuxième cylindre
    glm::mat4 cylinder4_transform = glm::mat4(1.0f);
    // Positionner le deuxième cylindre plus haut (au-dessus du premier)
    cylinder4_transform = glm::translate(cylinder4_transform, glm::vec3(-0.5f, 0.16f, -0.65f)); // 3.5 unités au-dessus de la voiture
    cylinder4_transform = glm::rotate(cylinder4_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder4_node = new Node(cylinder4_transform);
    cylinder4_node->add(cylinder4);

    // Ajouter le deuxième cylindre comme enfant du nœud voiture
    voiture_node->add(cylinder4_node);
    



    viewer.scene_root->add(voiture_node);
    viewer.voiture_node = voiture_node;  // permet de link la voiture au viewer (deplacement)

    // Lancer la boucle de rendu
    viewer.run();
}
