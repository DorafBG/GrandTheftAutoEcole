#include "viewer.h"

#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <voiture.h>

Viewer::Viewer(int width, int height)
{
    if (!glfwInit())    // initialize window system glfw
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    // version hints: create GL window with >= OpenGL 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    win = glfwCreateWindow(width, height, "Viewer", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    // make win's OpenGL context current; no OpenGL calls can happen before
    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    // Set user pointer for GLFW window to this Viewer instance
    glfwSetWindowUserPointer(win, this);

    // register event handlers
    glfwSetKeyCallback(win, key_callback_static);

    // event souris
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(win, cursor_position_callback_static);


    // useful message to check OpenGL renderer characteristics
    std::cout << glGetString(GL_VERSION) << ", GLSL "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << ", Renderer "
              << glGetString(GL_RENDERER) << std::endl;

    // initialize GL by setting viewport and default render characteristics
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable( GL_DEPTH_TEST ); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc( GL_LESS );

    // initialize our scene_root
    scene_root = new Node();
}

void Viewer::run()
{
    // Main render loop for this OpenGL window
    while (!glfwWindowShouldClose(win))
    {
        // clear draw buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // Limiter le delta_time pour éviter les sauts trop grands
        if (delta_time > 0.1f)
            delta_time = 0.1f;

        //Gestion clavier (PERMET DE NAVIGUER SUR LA MAP : UTILE POUR DEBUG)
        /*
        float velocity = camera_speed * delta_time;
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            camera_pos += velocity * camera_front;
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            camera_pos -= velocity * camera_front;
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * velocity;
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * velocity; */

        float vitesse = 3.0f * delta_time;        // vitesse de deplacement
        float vitesse_rotation = 90.0f * delta_time; // degres/s lors de la pivotation gauche/droite

        // On gere la voiture ici :
        glm::mat4 transform = voiture_node->getTransform(); //matrice de transformation actuelle de la voiture
        glm::vec3 direction = -glm::normalize(glm::vec3(transform[2][0], 0.0f, transform[2][2])); //direction actuelle
        glm::vec3 position = glm::vec3(transform[3][0], transform[3][1], transform[3][2]); //position actuelle
        glm::vec3 right = glm::normalize(glm::vec3(transform[0][0], 0.0f, transform[0][2]));//Direction perpendiculaire
        float currentYRotation = atan2(transform[2][0], transform[2][2]); // ROTATION actuelle de la voiture

        // GESTION DU DEPLACEMENT DE LA VOITURE
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS)
        {
            // la voiture avance
            position += direction * vitesse;
        }

        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        {
            // la voiture recule
            position -= direction * vitesse;
        }

        // Mettre à jour l'angle de rotation si nécessaire
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS)
        {
            // On tourne a gauche
            currentYRotation += glm::radians(vitesse_rotation);
        }

        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        {
            // On tourne a droite
            currentYRotation -= glm::radians(vitesse_rotation);
        }

        // On reconstruit la matrice de transformation de la voiture :
        glm::mat4 newTransform = glm::mat4(1.0f);
        newTransform = glm::translate(newTransform, position); //nouvelle position
        newTransform = glm::rotate(newTransform, currentYRotation, glm::vec3(0.0f, 1.0f, 0.0f));//nouvelle rotation
        glm::vec3 scale; // on reprend la meme echelle
        scale.x = glm::length(glm::vec3(transform[0][0], transform[0][1], transform[0][2]));
        scale.y = glm::length(glm::vec3(transform[1][0], transform[1][1], transform[1][2]));
        scale.z = glm::length(glm::vec3(transform[2][0], transform[2][1], transform[2][2]));
        newTransform = glm::scale(newTransform, scale);
        // Puis on applique la nouvelle transformation de la voiture
        voiture_node->setTransform(newTransform);

        // On met a jour la position de la camera
        glm::vec3 voiture_camera_offset(-6.0f, 15.0f, 0.0f); // pos de la cam par rapport a la voiture (10 de hauteur, 5 vers le bas)
        camera_pos = position + voiture_camera_offset;
        // il faut changer le front camera

        // LOGS VOITURE
        glm::mat4 voiture_transform = voiture_node->getTransform();
        glm::vec3 voiture_pos(voiture_transform[3][0], voiture_transform[3][1], voiture_transform[3][2]);
        std::cout << "Voiture position: ("
            << voiture_pos.x << ", "
            << voiture_pos.y << ", "
            << voiture_pos.z << ")\n";

        // LOGS CAMERA
        std::cout << "Camera position: ("
            << camera_pos.x << ", "
            << camera_pos.y << ", "
            << camera_pos.z << ")\n";

        std::cout << "Camera front: ("
            << camera_front.x << ", "
            << camera_front.y << ", "
            << camera_front.z << ")\n";


        glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

        float renderDistance = 500.0f;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, renderDistance);

        scene_root->draw(model, view, projection);

        // Poll for and process events
        glfwPollEvents();

        // flush render commands, and swap draw buffers
        glfwSwapBuffers(win);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
}

void Viewer::cursor_position_callback_static(GLFWwindow* window, double xpos, double ypos)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_mouse_move(xpos, ypos);
}

void Viewer::on_mouse_move(double xpos, double ypos)
{
    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // inversé car coordonnées y montent vers le bas

    last_x = xpos;
    last_y = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}


void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key);
}

void Viewer::on_key(int key)
{
    // 'Q' or 'Escape' quits
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}
