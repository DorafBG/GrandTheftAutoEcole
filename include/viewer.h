#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "node.h"

class Viewer {
public:
    Viewer(int width=640, int height=480);

    void run();
    void on_key(int key);

    Node *scene_root;

    // Camera control
    glm::vec3 camera_pos = glm::vec3(-4.73608f, 22.9012f, -3.91268f); // coordonnees camera
    glm::vec3 camera_front = glm::vec3(0.0174523f, -0.999848f, -0.000001f);//la camera regarde vers le sol (quand je mets x=0 et z=0, elle ne regarde plus le sol jsp pk)
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = -15.0f;
    float last_x = 320.0f;
    float last_y = 240.0f;
    bool first_mouse = true;
    float camera_speed = 20.5f;

    float delta_time = 0.0f;
    float last_frame = 0.0f;

    static void cursor_position_callback_static(GLFWwindow* window, double xpos, double ypos);
    void on_mouse_move(double xpos, double ypos);


private:
    GLFWwindow* win;
    static void key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif // VIEWER_H
