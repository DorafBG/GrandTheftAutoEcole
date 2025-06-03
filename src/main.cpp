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
const float rotationSpeed = 20.0f; // Degr�s par seconde

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cylinderRotation = 20.0f; // Rotation fixe � 20�
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cylinderRotation = -20.0f; // Rotation fixe � -20�
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

    // VOITURE (rectangle pos� l� o� la cam�ra regarde)
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
    // Cr�er un shader pour le cylindre
    Shader* cylinder_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");



    /// PREMIER CYLINDRE
    Cylinder* cylinder = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // shader, hauteur=2, rayon=0.5, 16 segments

    // Cr�er un n�ud pour le cylindre avec une transformation relative � la voiture
    glm::mat4 cylinder_transform = glm::mat4(1.0f);
    // Positionner le cylindre au-dessus de la voiture
    cylinder_transform = glm::translate(cylinder_transform, glm::vec3(-0.5f, 0.16f, 0.6f)); // 1.5 unit�s au-dessus
    cylinder_transform = glm::rotate(cylinder_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder_node = new Node(cylinder_transform);
    cylinder_node->add(cylinder);

    voiture_node->add(cylinder_node);

    float angle = viewer.angleRoues;


    // DEUXI�ME CYLINDRE
    Cylinder* cylinder2 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Cr�er un n�ud pour le deuxi�me cylindre
    glm::mat4 cylinder2_transform = glm::mat4(1.0f);
    // Positionner le deuxi�me cylindre plus haut (au-dessus du premier)
    cylinder2_transform = glm::translate(cylinder2_transform, glm::vec3(0.5f, 0.16f, -0.65f)); // 3.5 unit�s au-dessus de la voiture
    cylinder2_transform = glm::rotate(cylinder2_transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Node* cylinder2_node = new Node(cylinder2_transform);
    cylinder2_node->add(cylinder2);

    // Ajouter le deuxi�me cylindre comme enfant du n�ud voiture
    voiture_node->add(cylinder2_node);



    // 3e CYLINDRE
    Cylinder* cylinder3 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Cr�er un n�ud pour le deuxi�me cylindre
    glm::mat4 cylinder3_transform = glm::mat4(1.0f);
    // Positionner le deuxi�me cylindre plus haut (au-dessus du premier)
    cylinder3_transform = glm::translate(cylinder3_transform, glm::vec3(0.5f, 0.16f, 0.6f)); // 3.5 unit�s au-dessus de la voiture
    cylinder3_transform = glm::rotate(cylinder3_transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder3_node = new Node(cylinder3_transform);
    cylinder3_node->add(cylinder3);

    // Ajouter le deuxi�me cylindre comme enfant du n�ud voiture
    voiture_node->add(cylinder3_node);


    // 4e CYLINDRE
    Cylinder* cylinder4 = new Cylinder(cylinder_shader, 0.2f, 0.16f, 16); // plus petit: hauteur=1.5, rayon=0.4

    // Cr�er un n�ud pour le deuxi�me cylindre
    glm::mat4 cylinder4_transform = glm::mat4(1.0f);
    // Positionner le deuxi�me cylindre plus haut (au-dessus du premier)
    cylinder4_transform = glm::translate(cylinder4_transform, glm::vec3(-0.5f, 0.16f, -0.65f)); // 3.5 unit�s au-dessus de la voiture
    cylinder4_transform = glm::rotate(cylinder4_transform, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Node* cylinder4_node = new Node(cylinder4_transform);
    cylinder4_node->add(cylinder4);

    // Ajouter le deuxi�me cylindre comme enfant du n�ud voiture
    voiture_node->add(cylinder4_node);

    
   /* Cube fonctionnel :
   
   
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
    Cube* mon_cube = new Cube(color_shader, glm::vec3(1.0f, 0.2f, 0.2f)); // Par exemple


    // Matrice de transformation : d'abord l'échelle, puis la translation
    glm::mat4 mat_cube = glm::mat4(1.0f);
    mat_cube = glm::translate(mat_cube, glm::vec3(0, 5, 0));           // 5 unités plus haut
    mat_cube = glm::scale(mat_cube, glm::vec3(4.0f, 4.0f, 4.0f));      // Cube 3x plus grand


    // Crée un Node pour la transformation
    Node* node_cube = new Node(mat_cube);

    // Ajoute le cube au Node
    node_cube->add(mon_cube);

    // Ajoute le Node à la racine de la scène
    viewer.scene_root->add(node_cube);


    // 1. Charger le shader couleur uniforme
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");

    // 2. Définir une couleur verte (par exemple, vert vif)
    glm::vec3 couleur_verte(0.0f, 1.0f, 0.0f); // Vert pur

    // 3. Instancier le cube avec cette couleur
    Cube* cube_vert = new Cube(color_shader, couleur_verte);

    // 4. Créer la matrice de transformation (par exemple, posé sur le sol à x=0, y=1, z=0)
    glm::mat4 mat_cube = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0));

    // 5. Créer le Node pour la scène
    Node* node_cube = new Node(mat_cube);

    // 6. Ajouter le cube au Node
    node_cube->add(cube_vert);

    // 7. Ajouter le Node à la racine de la scène
    viewer.scene_root->add(node_cube);*/

    // Charger le shader couleur uniforme
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
/*
    // Cube vert (à gauche, par exemple)
    glm::vec3 couleur_verte(0.0f, 1.0f, 0.0f); // Vert pur
    Cube* cube_vert = new Cube(color_shader, couleur_verte);
    glm::mat4 mat_cube_vert = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 3, 0)); // à gauche
    mat_cube_vert = glm::scale(mat_cube_vert, glm::vec3(4.0f, 4.0f, 4.0f));
    Node* node_cube_vert = new Node(mat_cube_vert);
    node_cube_vert->add(cube_vert);
    viewer.scene_root->add(node_cube_vert);

    // Cube rouge (à droite, par exemple)
    glm::vec3 couleur_rouge(1.0f, 0.0f, 0.0f); // Rouge pur
    Cube* cube_rouge = new Cube(color_shader, couleur_rouge);
    glm::mat4 mat_cube_rouge = glm::translate(glm::mat4(1.0f), glm::vec3(4, 3, 0)); // à droite
    mat_cube_rouge = glm::scale(mat_cube_rouge, glm::vec3(4.0f, 4.0f, 4.0f));
    Node* node_cube_rouge = new Node(mat_cube_rouge);
    node_cube_rouge->add(cube_rouge);
    viewer.scene_root->add(node_cube_rouge);*/


        // Crée un shader pour chaque (ou réutilise le même)
    Shader* shader_maison = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");

    // Crée la maison : cube bleu, toit rouge par exemple
    Maison* maison = new Maison(shader_maison, shader_maison, 
        glm::vec3(0.2f, 0.2f, 1.0f),    // bleu pour le cube
        glm::vec3(1.0f, 0.8f, 0.0f));   // jaune/orange pour le toit

    // Positionne la maison (ex : à y = 1 pour qu’elle soit posée sur le sol)
    glm::mat4 mat_maison = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0)); // Translation d'abord
    mat_maison = glm::scale(mat_maison, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

    Node* node_maison = new Node(mat_maison);
    node_maison->add(maison);

    Shader* shader_blanc = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");

    // Instancie une éolienne blanche
    Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f)); // blanc

    // Positionne l’éolienne dans la scène (par exemple à x=5, z=0)
    glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(5, 0, 0));
    Node* node_eolienne = new Node(mat_eolienne);
    node_eolienne->add(eolienne);
    viewer.scene_root->add(node_eolienne);

    viewer.eoliennes.push_back(eolienne);

    // 3. Crée une DEUXIÈME éolienne à une position différente
    Eolienne* eolienne2 = new Eolienne(shader_blanc, glm::vec3(1.0f)); // toujours blanc


    // 4. Place la deuxième éolienne à une autre position, par exemple (x=10, y=0, z=5)
    glm::mat4 mat_eolienne2 = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 5));
    Node* node_eolienne2 = new Node(mat_eolienne2);
    node_eolienne2->add(eolienne2);
    viewer.scene_root->add(node_eolienne2);

    // 5. Ajoute la deuxième éolienne à la liste des éoliennes à mettre à jour
    viewer.eoliennes.push_back(eolienne2);

    viewer.scene_root->add(node_maison);
    




    viewer.scene_root->add(voiture_node);
    viewer.voiture_node = voiture_node;  // permet de link la voiture au viewer (deplacement)

    // Lancer la boucle de rendu
    viewer.run();
}
