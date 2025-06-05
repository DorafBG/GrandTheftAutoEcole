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
#include "panneau.h"
#include "cubesimple.h"
#include "banc.h"
#include "arbre.h"
#include <vector>
#include <cstdlib> 
#include <ctime> 
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
std::vector<Arbre*> arbres;
std::vector<Node*> arbres_nodes;

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
    srand(time(0));
    Viewer viewer;
    std::string shader_dir = SHADER_DIR;
    std::string texture_dir = TEXTURE_DIR;

     Shader* phong_shader = new Shader(
        shader_dir + "phong.vert",
        shader_dir + "phong.frag"
    );
    viewer.phong_shader = phong_shader;

    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // tapis
    Texture* carpet_texture = new Texture(texture_dir + "tapis.jpg");
    Shader* ground_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    Shape* ground = new Ground(ground_shader, carpet_texture);
    glm::mat4 ground_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f)); //50 fois plus large et profond (pas en hauteur)
    Node* ground_node = new Node(ground_mat);
    ground_node->add(ground);
    viewer.scene_root->add(ground_node);


    // SOL
    Texture* grass_texture = new Texture(texture_dir + "grass.jpg");

    Shape* ground2 = new Ground(ground_shader, grass_texture);
    glm::mat4 ground_mat2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.05f, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(300.0f, 1.0f, 300.0f)); //50 fois plus large et profond (pas en hauteur)
    Node* ground_node2 = new Node(ground_mat2);
    ground_node2->add(ground2);
    viewer.scene_root->add(ground_node2);
    // VOITURE
    glm::vec3 cam_pos = viewer.camera_pos;
    glm::vec3 cam_dir = glm::normalize(viewer.camera_front);
    float t = -cam_pos.y / cam_dir.y;
    glm::vec3 intersection_point = cam_pos + t * cam_dir;

    Shader* car_shader = new Shader(shader_dir + "textureVoiture.vert", shader_dir + "textureVoiture.frag");
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

    Shader* shader_maison = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");

   //maison1
   Texture* mur_texture1 = new Texture(texture_dir + "mur_marron.png");
   // Crée la maison : cube bleu, toit rouge par exemple
   Maison* maison1 = new Maison(texture_shader, shader_maison, 
       mur_texture1/*glm::vec3(0.2f, 0.2f, 1.0f)*/,
       glm::vec3(0.5f, 0.1f, 0.1f), //couleur toit
       1.7f, 2.3f, 0.5f, 0.5f, 1);  //longueur, largeur, hauteur_mur, hauteur_toit, choix du toit

   // Positionne la maison (ex : à y = 1 pour qu’elle soit posée sur le sol)
   glm::mat4 mat_maison1 = glm::translate(glm::mat4(1.0f), glm::vec3(-30, 1, 9.2)); // Translation = 2*hauteur_mur
   mat_maison1 = glm::scale(mat_maison1, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison1 = new Node(mat_maison1); node_maison1->add(maison1); viewer.scene_root->add(node_maison1);



   //maison2
   Texture* mur_texture2 = new Texture(texture_dir + "hlm.png");
   // Crée la maison : cube bleu, toit rouge par exemple
   Maison* maison2 = new Maison(texture_shader, shader_maison,
       mur_texture2/*glm::vec3(0.2f, 0.2f, 1.0f)*/,
       glm::vec3(0.0f, 0.0f, 0.0f), //couleur toit
       2.5f, 3.0f, 6.0f, 0.0f, 0);  //longueur largeur hauteur_mur hauteur_toit

   // Positionne la maison (ex : à y = 1 pour qu’elle soit posée sur le sol)
   glm::mat4 mat_maison2 = glm::translate(glm::mat4(1.0f), glm::vec3(-12.5, 12, -16)); // Translation = 2*hauteur_mur
   mat_maison2 = glm::scale(mat_maison2, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison2 = new Node(mat_maison2); node_maison2->add(maison2); viewer.scene_root->add(node_maison2);



   //maison4
   Texture* mur_texture4 = new Texture(texture_dir + "lego.png");
   Maison* maison4 = new Maison(texture_shader, shader_maison,
       mur_texture4,
       glm::vec3(0.2f, 0.2f, 0.2f),
       1.7f, 2.0f, 1.5f, 1.0f, 3);  //longueur largeur hauteur_mur hauteur_toit

   // Positionne la maison (ex : à y = 1 pour qu’elle soit posée sur le sol)
   glm::mat4 mat_maison4 = glm::translate(glm::mat4(1.0f), glm::vec3(-42.5, 3, 8.8)); // Translation = 2*hauteur_mur
   mat_maison4 = glm::scale(mat_maison4, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison4 = new Node(mat_maison4); node_maison4->add(maison4); viewer.scene_root->add(node_maison4);


   //maison5
   Texture* mur_texture5 = new Texture(texture_dir + "newyork.png");
   Maison* maison5 = new Maison(texture_shader, shader_maison,
       mur_texture5,
       glm::vec3(0.5f, 0.2f, 0.2f),
       1.7f, 1.9f, 4.0f, 0.7f, 0);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison5 = glm::translate(glm::mat4(1.0f), glm::vec3(9, 8, 1)); // Translation = 2*hauteur_mur
   mat_maison5 = glm::scale(mat_maison5, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison5 = new Node(mat_maison5); node_maison5->add(maison5); viewer.scene_root->add(node_maison5);


   //maison6
   Texture* mur_texture6 = new Texture(texture_dir + "newyork2.jpg");
   Maison* maison6 = new Maison(texture_shader, shader_maison,
       mur_texture6,
       glm::vec3(0.5f, 0.2f, 0.2f),
       1.3f, 1.8f, 3.5f, 0.7f, 0);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison6 = glm::translate(glm::mat4(1.0f), glm::vec3(7.5, 7, -8.2)); // Translation = 2*hauteur_mur
   mat_maison6 = glm::scale(mat_maison6, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison6 = new Node(mat_maison6); node_maison6->add(maison6); viewer.scene_root->add(node_maison6);


   //maison7
   Texture* mur_texture7 = new Texture(texture_dir + "house.jpg");
   Maison* maison7 = new Maison(texture_shader, shader_maison,
       mur_texture7,
       glm::vec3(0.4f, 0.3f, 0.2f),
       1.3f, 2.0f, 0.7f, 0.8f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison7 = glm::translate(glm::mat4(1.0f), glm::vec3(22.7, 1.4, 10.5)); // Translation = 2*hauteur_mur
   mat_maison7 = glm::scale(mat_maison7, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison7 = new Node(mat_maison7); node_maison7->add(maison7); viewer.scene_root->add(node_maison7);


   //maison8
   Texture* mur_texture8 = new Texture(texture_dir + "bluehouse.jpg");
   Maison* maison8 = new Maison(texture_shader, shader_maison,
       mur_texture8,
       glm::vec3(0.1f, 0.1f, 0.3f),
       1.3f, 1.8f, 1.0f, 0.7f, 3);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison8 = glm::translate(glm::mat4(1.0f), glm::vec3(29.5, 2, 2)); // Translation = 2*hauteur_mur
   mat_maison8 = glm::scale(mat_maison8, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison8 = new Node(mat_maison8); node_maison8->add(maison8); viewer.scene_root->add(node_maison8);


   //maison9
   Texture* mur_texture9 = new Texture(texture_dir + "hlm.png");
   Maison* maison9 = new Maison(texture_shader, shader_maison,
       mur_texture9,
       glm::vec3(1.0f, 0.2f, 0.2f),
       1.3f, 1.8f, 3.1f, 0.7f, 0);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison9 = glm::translate(glm::mat4(1.0f), glm::vec3(41, 6.2, 1)); // Translation = 2*hauteur_mur
   mat_maison9 = glm::scale(mat_maison9, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison9 = new Node(mat_maison9); node_maison9->add(maison9); viewer.scene_root->add(node_maison9);


   //maison10
   Texture* mur_texture10 = new Texture(texture_dir + "yellowhouse.jpg");
   Maison* maison10 = new Maison(texture_shader, shader_maison,
       mur_texture10,
       glm::vec3(0.3f, 0.2f, 0.1f),
       1.2f, 1.9f, 2.2f, 0.2f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison10 = glm::translate(glm::mat4(1.0f), glm::vec3(47, 4.4, -42)); // Translation = 2*hauteur_mur
   mat_maison10 = glm::scale(mat_maison10, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison10 = new Node(mat_maison10); node_maison10->add(maison10); viewer.scene_root->add(node_maison10);


   //maison11
   Texture* mur_texture11 = new Texture(texture_dir + "yellowhouse.jpg");
   Maison* maison11 = new Maison(texture_shader, shader_maison,
       mur_texture11,
       glm::vec3(0.2f, 0.1f, 0.1f),
       1.2f, 1.9f, 2.3, 0.3f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison11 = glm::translate(glm::mat4(1.0f), glm::vec3(41, 4.6, 19)); // Translation = 2*hauteur_mur
   mat_maison11 = glm::scale(mat_maison11, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison11 = new Node(mat_maison11); node_maison11->add(maison11); viewer.scene_root->add(node_maison11);


   //maison12
   Texture* mur_texture12 = new Texture(texture_dir + "house.jpg");
   Maison* maison12 = new Maison(texture_shader, shader_maison,
       mur_texture12,
       glm::vec3(0.0f, 0.1f, 0.1f),
       1.5f, 1.5f, 1.8f, 0.7f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison12 = glm::translate(glm::mat4(1.0f), glm::vec3(5, 3.6, -34.5)); // Translation = 2*hauteur_mur
   mat_maison12 = glm::scale(mat_maison12, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison12 = new Node(mat_maison12); node_maison12->add(maison12); viewer.scene_root->add(node_maison12);


   //maison13
   Texture* mur_texture13 = new Texture(texture_dir + "house.jpg");
   Maison* maison13 = new Maison(texture_shader, shader_maison,
       mur_texture13,
       glm::vec3(0.0f, 0.1f, 0.1f),
       0.6f, 0.7f, 0.7f, 0.3f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison13 = glm::translate(glm::mat4(1.0f), glm::vec3(10.5, 1.4, -34.5)); // Translation = 2*hauteur_mur
   mat_maison13 = glm::scale(mat_maison13, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison13 = new Node(mat_maison13); node_maison13->add(maison13); viewer.scene_root->add(node_maison13);


   //maison14
   Texture* mur_texture14 = new Texture(texture_dir + "manoir.png");
   Maison* maison14 = new Maison(texture_shader, shader_maison,
       mur_texture14,
       glm::vec3(0.4f, 0.3f, 0.3f),
       3.5f, 2.0f, 2.0f, 0.7f, 3);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison14 = glm::translate(glm::mat4(1.0f), glm::vec3(13, 4, 42.5)); // Translation = 2*hauteur_mur
   mat_maison14 = glm::scale(mat_maison14, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison14 = new Node(mat_maison14); node_maison14->add(maison14); viewer.scene_root->add(node_maison14);


   //maison15
   Texture* mur_texture15 = new Texture(texture_dir + "brick.jpg");
   Maison* maison15 = new Maison(texture_shader, shader_maison,
       mur_texture15,
       glm::vec3(0.3f, 0.3f, 0.3f),
       1.2f, 1.4f, 1.0f, 2.5f, 1);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison15 = glm::translate(glm::mat4(1.0f), glm::vec3(2, 2, 25)); // Translation = 2*hauteur_mur
   mat_maison15 = glm::scale(mat_maison15, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison15 = new Node(mat_maison15); node_maison15->add(maison15); viewer.scene_root->add(node_maison15);


   //maison16
   Texture* mur_texture16 = new Texture(texture_dir + "manoir.png");
   Maison* maison16 = new Maison(texture_shader, shader_maison,
       mur_texture16,
       glm::vec3(0.6f, 0.6f, 0.5f),
       4.7f, 4.7f, 2.7f, 0.8f, 3);  //longueur largeur hauteur_mur hauteur_toit

   glm::mat4 mat_maison16 = glm::translate(glm::mat4(1.0f), glm::vec3(-22.5, 5.2, -44)); // Translation = 2*hauteur_mur
   mat_maison16 = glm::scale(mat_maison16, glm::vec3(4.0f, 4.0f, 4.0f));           // Puis scale

   Node* node_maison16 = new Node(mat_maison16); node_maison16->add(maison16); viewer.scene_root->add(node_maison16);

   


    

    // shader des éoliennes
    Shader* shader_blanc = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
   

    // Cage de foot
    CageDeFoot* cage = new CageDeFoot(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_cage = glm::translate(glm::mat4(1.0f), glm::vec3(10, 0, 0)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_cage = new Node(mat_cage); node_cage->add(cage); viewer.scene_root->add(node_cage);

    // Feu de circulation
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
    TrafficLight* traffic_light = new TrafficLight(color_shader);
    glm::mat4 mat_traffic_light = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
    Node* node_traffic_light = new Node(mat_traffic_light);
    node_traffic_light->add(traffic_light);
    viewer.scene_root->add(node_traffic_light);
    viewer.traffic_lights.push_back(traffic_light);



    //Soleil
    glm::vec3 soleil_pos(60, 15, 15);
    glm::vec3 soleil_couleur(1.0f, 1.0f, 0.8f);
    glm::vec3 sphere_jaune(1.0f, 1.0f, 0.2f);

    LightingSphere* soleil = new LightingSphere(phong_shader, soleil_pos, soleil_couleur, sphere_jaune);
    glm::mat4 mat_soleil = glm::translate(glm::mat4(1.0f), soleil_pos) * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));
    Node* node_soleil = new Node(mat_soleil);
    node_soleil->add(soleil);
    viewer.scene_root->add(node_soleil);




    // Ajout final de la voiture
    viewer.scene_root->add(voiture_node);
    viewer.voiture_node = voiture_node;

    //banc 
    Shader* uniform_shader = new Shader(shader_dir + "uniform_color.vert",shader_dir + "uniform_color.frag");
    Banc* banc = new Banc(shader_blanc,glm::vec3(1.0f));
    glm::mat4 mat_banc = glm::translate(glm::mat4(1.0f), glm::vec3(15,-0.35f,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc = new Node(mat_banc);
    node_banc ->add(banc);
    viewer.scene_root->add(node_banc);

    Arbre* arbre = new Arbre(uniform_shader);
    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(20,0,0))  * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_arbre = new Node(mat_arbre);
    node_arbre ->add(arbre);
    viewer.scene_root->add(node_arbre);

    std::vector<Arbre*> arbres;

    Arbre* arbre2 = new Arbre(uniform_shader);
    glm::mat4 mat_arbre2 = glm::translate(glm::mat4(1.0f), glm::vec3(50,0,0))  * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_arbre2 = new Node(mat_arbre2);
    node_arbre2 ->add(arbre2);
    viewer.scene_root->add(node_arbre2);


    // Création des arbres et de leur node parent
    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire
    float x = 50.0f + static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = static_cast<float>(rand()) / RAND_MAX * 200.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire 
    float x = -50.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = static_cast<float>(rand()) / RAND_MAX * 200.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire
    float x = 15.0f + static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = static_cast<float>(rand()) / RAND_MAX * 200.0f;;         // Z entre -10 et +9

    // taille aléatoire
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = -55.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = static_cast<float>(rand()) / RAND_MAX * 300.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = -55.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = -45.0f - static_cast<float>(rand()) / RAND_MAX * 200.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = 11.6f + static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = -82.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = -17 + static_cast<float>(rand()) / RAND_MAX * 200.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = 82.0f + static_cast<float>(rand()) / RAND_MAX * 100.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = -66 - static_cast<float>(rand()) / RAND_MAX * 200.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = -22.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }

    for (int i = 0; i < 30; ++i) {
    Arbre* arbre = new Arbre(uniform_shader);

    // Position aléatoire autour de X=30, Z aléatoire, Y=0 (au sol)
    float x = -37.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f; // X entre 20 et 40
    float y = 0.0f;
    float z = -73.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;;         // Z entre -10 et +9

    // Echelle aléatoire (optionnel, pour un peu de variété)
    float scale = 1.5f + (rand() % 100) / 100.0f; // entre 1.5 et 2.5

    glm::mat4 mat_arbre = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    Node* node_arbre = new Node(mat_arbre);
    node_arbre->add(arbre);

    viewer.scene_root->add(node_arbre);
    }


    for (int i = 0; i < 15; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = 50.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f;
        float y = 0.0f;
        float z = -10.0f + static_cast<float>(rand()) / RAND_MAX * 100.0f;;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

     for (int i = 0; i < 15; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = 62.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f;
        float y = 0.0f;
        float z = -10.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }


    for (int i = 0; i < 30; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = 50.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;
        float y = 0.0f;
        float z = -50.0f - static_cast<float>(rand()) / RAND_MAX * 200.0f;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

    for (int i = 0; i < 30; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = -50.0f + static_cast<float>(rand()) / RAND_MAX * 250.0f;
        float y = 0.0f;
        float z = -50.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

    for (int i = 0; i < 30; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = -50.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;
        float y = 0.0f;
        float z = 35.0f - static_cast<float>(rand()) / RAND_MAX * 150.0f;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

    for (int i = 0; i < 30; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = 30.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;
        float y = 0.0f;
        float z = 67.0f + static_cast<float>(rand()) / RAND_MAX * 150.0f;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

    for (int i = 0; i < 30; ++i) {
        Eolienne* eolienne = new Eolienne(shader_blanc, glm::vec3(1.0f, 1.0f, 1.0f));

        // Position aléatoire
        float x = 50.0f - static_cast<float>(rand()) / RAND_MAX * 100.0f;
        float y = 0.0f;
        float z = 50.0f + static_cast<float>(rand()) / RAND_MAX * 150.0f;

        glm::mat4 mat_eolienne = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
        Node* node_eolienne = new Node(mat_eolienne);

        node_eolienne->add(eolienne);
        viewer.scene_root->add(node_eolienne);

        viewer.eoliennes.push_back(eolienne);
    }

    Texture* texture_panneau = new Texture(texture_dir + "stop2.png");  

    Panneau* panneau = new Panneau(color_shader, glm::vec3(0.3f, 0.3f, 0.3f), color_shader, texture_panneau);

    glm::mat4 mat_panneau = glm::translate(glm::mat4(1.0f), glm::vec3(20, 0, -40));
    mat_panneau = glm::scale(mat_panneau, glm::vec3(1.1f, 1.1f , 1.1f)); 
    Node* node_panneau = new Node(mat_panneau);
    node_panneau->add(panneau);
    viewer.scene_root->add(node_panneau);

    Texture* texture_panneau2 = new Texture(texture_dir + "panneau_voiture.png"); 

    Panneau* panneau2 = new Panneau(color_shader, glm::vec3(0.3f, 0.3f, 0.3f), color_shader, texture_panneau);

    glm::mat4 mat_panneau2 = glm::translate(glm::mat4(1.0f), glm::vec3(25, 0, -40));
    mat_panneau2 = glm::scale(mat_panneau2, glm::vec3(1.1f, 1.1f , 1.1f)); 
    Node* node_panneau2 = new Node(mat_panneau2);
    node_panneau2->add(panneau2);
    viewer.scene_root->add(node_panneau2);








    viewer.run();
}
