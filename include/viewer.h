#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"
#include <vector>
#include "eolienne.h"
#include "traffic_light.h"
#include "arbre.h"

class Viewer {
public:
    Viewer(int width = 640, int height = 480);

    void run();
    void on_key(int key);

    Node* scene_root;

    // Camera control
    glm::vec3 camera_pos = glm::vec3(-17.4414f, 20.0f, 5.0f);
    glm::vec3 camera_front = glm::vec3(0.0f, -0.933f, -0.360f);


    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    Node* voiture_node;  // pointeur vers le noeud de la voiture
    float last_x = 320.0f;
    float last_y = 240.0f;
    bool first_mouse = true;
    float camera_speed = 20.5f;

    float camera_distance = 10.0f;
    float camera_height_offset = 3.0f;
    float camera_yaw = 180.0f;
    float camera_pitch = 10.0f;

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    float angleRoues = 0.0f;

    static void cursor_position_callback_static(GLFWwindow* window, double xpos, double ypos);
    void on_mouse_move(double xpos, double ypos);

    std::vector<Eolienne*> eoliennes;
    std::vector<TrafficLight*> traffic_lights;

    Shader* phong_shader = nullptr;



private:
    GLFWwindow* win;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif // VIEWER_H
