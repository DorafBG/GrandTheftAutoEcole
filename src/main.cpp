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
        glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f)); 
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
 //50 fois plus large et profond (pas en hauteur)w

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
   

    // Première cage de foot
    CageDeFoot* cage1 = new CageDeFoot(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_cage1 = glm::translate(glm::mat4(1.0f), glm::vec3(-33.6549f, 0.0f, -0.0345873f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_cage1 = new Node(mat_cage1);
    node_cage1->add(cage1);
    viewer.scene_root->add(node_cage1);

    // Deuxième cage de foot
    CageDeFoot* cage2 = new CageDeFoot(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_cage2 = glm::translate(glm::mat4(1.0f), glm::vec3(-34.7598f, 0.0f, -18.6959f)) * glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
    Node* node_cage2 = new Node(mat_cage2);
    node_cage2->add(cage2);
    viewer.scene_root->add(node_cage2);

    // Feu de circulation
    Shader* color_shader = new Shader(shader_dir + "uniform_color.vert", shader_dir + "uniform_color.frag");
    TrafficLight* traffic_light = new TrafficLight(color_shader);
    glm::mat4 mat_traffic_light = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f));
    Node* node_traffic_light = new Node(mat_traffic_light);
    node_traffic_light->add(traffic_light);
    viewer.scene_root->add(node_traffic_light);
    viewer.traffic_lights.push_back(traffic_light);

std::vector<glm::vec3> traffic_light_positions = {
    glm::vec3(-18.4f, 0.0f, -0.26f),
    glm::vec3(7.3f, 0.0f, 7.7f),
    glm::vec3(45.0f, 0.0f, 38.0f),
    glm::vec3(44.4f, 0.0f, -23.97f),
    glm::vec3(-15.15f, 0.0f, -29.2f),
};

for(const auto& pos : traffic_light_positions) {
    TrafficLight* traffic_light = new TrafficLight(color_shader);
    glm::mat4 mat_traffic_light = glm::translate(glm::mat4(1.0f), pos);
    Node* node_traffic_light = new Node(mat_traffic_light);
    node_traffic_light->add(traffic_light);
    viewer.scene_root->add(node_traffic_light);
    viewer.traffic_lights.push_back(traffic_light);
}




    //Soleil
    glm::vec3 soleil_pos(60, 40, 15);
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

    Banc* banc1 = new Banc(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_banc1 = glm::translate(glm::mat4(1.0f), glm::vec3(30.6f, -0.35f, 17.77f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc1 = new Node(mat_banc1);
    node_banc1->add(banc1);
    viewer.scene_root->add(node_banc1);

    Banc* banc2 = new Banc(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_banc2 = glm::translate(glm::mat4(1.0f), glm::vec3(10.59f, -0.35f, 2.95f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc2 = new Node(mat_banc2);
    node_banc2->add(banc2);
    viewer.scene_root->add(node_banc2);

    Banc* banc3 = new Banc(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_banc3 = glm::translate(glm::mat4(1.0f), glm::vec3(21.951f, -0.35f, 4.4f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc3 = new Node(mat_banc3);
    node_banc3->add(banc3);
    viewer.scene_root->add(node_banc3);

    Banc* banc4 = new Banc(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_banc4 = glm::translate(glm::mat4(1.0f), glm::vec3(41.717f, -0.35f, 12.869f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc4 = new Node(mat_banc4);
    node_banc4->add(banc4);
    viewer.scene_root->add(node_banc4);

    Banc* banc5 = new Banc(shader_blanc, glm::vec3(1.0f));
    glm::mat4 mat_banc5 = glm::translate(glm::mat4(1.0f), glm::vec3(-39.57f, -0.35f, -29.14f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.25f));
    Node* node_banc5 = new Node(mat_banc5);
    node_banc5->add(banc5);
    viewer.scene_root->add(node_banc5);


/*
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
*/

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

    // Premier panneau STOP
    Panneau* panneau1 = new Panneau(uniform_shader, glm::vec3(0.7f, 0.7f, 0.7f), uniform_shader);
    glm::mat4 mat_panneau1 = glm::translate(glm::mat4(1.0f), glm::vec3(-37.19f, 0.0f, 20.2f));
    Node* node_panneau1 = new Node(mat_panneau1);
    node_panneau1->add(panneau1);
    viewer.scene_root->add(node_panneau1);

    // Deuxième panneau STOP
    Panneau* panneau2 = new Panneau(uniform_shader, glm::vec3(0.7f, 0.7f, 0.7f), uniform_shader);
    glm::mat4 mat_panneau2 = glm::translate(glm::mat4(1.0f), glm::vec3(-17.76f, 0.0f, -7.89f));
    Node* node_panneau2 = new Node(mat_panneau2);
    node_panneau2->add(panneau2);
    viewer.scene_root->add(node_panneau2);

    // Troisième panneau STOP
    Panneau* panneau3 = new Panneau(uniform_shader, glm::vec3(0.7f, 0.7f, 0.7f), uniform_shader);
    glm::mat4 mat_panneau3 = glm::translate(glm::mat4(1.0f), glm::vec3(26.67f, 0.0f, 25.16f));
    Node* node_panneau3 = new Node(mat_panneau3);
    node_panneau3->add(panneau3);
    viewer.scene_root->add(node_panneau3);


    // Fonction utilitaire pour générer un float entre min et max
    auto random_float = [](float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
    };

    // Arbre 1
    Arbre* arbre1 = new Arbre(uniform_shader);
    float taille1 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre1 = glm::translate(glm::mat4(1.0f), glm::vec3(-45.8f, 0.0f, 42.34f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille1));
    Node* node_arbre1 = new Node(mat_arbre1);
    node_arbre1->add(arbre1);
    viewer.scene_root->add(node_arbre1);

    // Arbre 2
    Arbre* arbre2 = new Arbre(uniform_shader);
    float taille2 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre2 = glm::translate(glm::mat4(1.0f), glm::vec3(-34.7f, 0.0f, 36.8f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille2));
    Node* node_arbre2 = new Node(mat_arbre2);
    node_arbre2->add(arbre2);
    viewer.scene_root->add(node_arbre2);

    // Arbre 3
    Arbre* arbre3 = new Arbre(uniform_shader);
    float taille3 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre3 = glm::translate(glm::mat4(1.0f), glm::vec3(-32.43f, 0.0f, 22.918f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille3));
    Node* node_arbre3 = new Node(mat_arbre3);
    node_arbre3->add(arbre3);
    viewer.scene_root->add(node_arbre3);

    // Arbre 4
    Arbre* arbre4 = new Arbre(uniform_shader);
    float taille4 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre4 = glm::translate(glm::mat4(1.0f), glm::vec3(-22.0f, 0.0f, 21.9f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille4));
    Node* node_arbre4 = new Node(mat_arbre4);
    node_arbre4->add(arbre4);
    viewer.scene_root->add(node_arbre4);

    // Arbre 5
    Arbre* arbre5 = new Arbre(uniform_shader);
    float taille5 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre5 = glm::translate(glm::mat4(1.0f), glm::vec3(-35.5f, 0.0f, 12.3f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille5));
    Node* node_arbre5 = new Node(mat_arbre5);
    node_arbre5->add(arbre5);
    viewer.scene_root->add(node_arbre5);

    // Arbre 6
    Arbre* arbre6 = new Arbre(uniform_shader);
    float taille6 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre6 = glm::translate(glm::mat4(1.0f), glm::vec3(-18.7f, 0.0f, 3.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille6));
    Node* node_arbre6 = new Node(mat_arbre6);
    node_arbre6->add(arbre6);
    viewer.scene_root->add(node_arbre6);

    // Arbre 7
    Arbre* arbre7 = new Arbre(uniform_shader);
    float taille7 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre7 = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, 0.0f, 16.02f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille7));
    Node* node_arbre7 = new Node(mat_arbre7);
    node_arbre7->add(arbre7);
    viewer.scene_root->add(node_arbre7);

    // Arbre 8
    Arbre* arbre8 = new Arbre(uniform_shader);
    float taille8 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre8 = glm::translate(glm::mat4(1.0f), glm::vec3(31.109f, 0.0f, 13.8f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille8));
    Node* node_arbre8 = new Node(mat_arbre8);
    node_arbre8->add(arbre8);
    viewer.scene_root->add(node_arbre8);

    // Arbre 9
    Arbre* arbre9 = new Arbre(uniform_shader);
    float taille9 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre9 = glm::translate(glm::mat4(1.0f), glm::vec3(41.3f, 0.0f, 7.6f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille9));
    Node* node_arbre9 = new Node(mat_arbre9);
    node_arbre9->add(arbre9);
    viewer.scene_root->add(node_arbre9);

    // Arbre 10
    Arbre* arbre10 = new Arbre(uniform_shader);
    float taille10 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre10 = glm::translate(glm::mat4(1.0f), glm::vec3(47.98f, 0.0f, -40.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille10));
    Node* node_arbre10 = new Node(mat_arbre10);
    node_arbre10->add(arbre10);
    viewer.scene_root->add(node_arbre10);

    // Arbre 11
    Arbre* arbre11 = new Arbre(uniform_shader);
    float taille11 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre11 = glm::translate(glm::mat4(1.0f), glm::vec3(29.2f, 0.0f, -11.15f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille11));
    Node* node_arbre11 = new Node(mat_arbre11);
    node_arbre11->add(arbre11);
    viewer.scene_root->add(node_arbre11);

    // Arbre 12
    Arbre* arbre12 = new Arbre(uniform_shader);
    float taille12 = random_float(1.5f, 2.5f);
    glm::mat4 mat_arbre12 = glm::translate(glm::mat4(1.0f), glm::vec3(-38.8f, 0.0f, -43.6f)) * glm::scale(glm::mat4(1.0f), glm::vec3(taille12));
    Node* node_arbre12 = new Node(mat_arbre12);
    node_arbre12->add(arbre12);
    viewer.scene_root->add(node_arbre12);








    viewer.run();
}
