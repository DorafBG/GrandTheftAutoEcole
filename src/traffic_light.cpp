#include "traffic_light.h"
#include "cube_traffic_light.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Constructeur
TrafficLight::TrafficLight(Shader* shader)
    : Shape(shader),  
      currentState_(RED),
      stateTime_(0.0f),
      shader_(shader)
{

    support_    = new Cylinder(shader_, 2.0f, 0.2f, 16);
    redCube_    = new CubeTrafficLight(shader_, glm::vec3(0.4f, 0.4f, 0.4f));
    orangeCube_ = new CubeTrafficLight(shader_, glm::vec3(0.4f, 0.4f, 0.4f));
    greenCube_  = new CubeTrafficLight(shader_, glm::vec3(0.4f, 0.4f, 0.4f));
}


TrafficLight::~TrafficLight() {
    delete support_;
    delete redCube_;
    delete orangeCube_;
    delete greenCube_;
}

// Fonction d'évolution de l'état (cycle feu)
void TrafficLight::update(float deltaTime) {
    stateTime_ += deltaTime;
    switch (currentState_) {
        case GREEN:
            if (stateTime_ >= 6.0f) {
                currentState_ = ORANGE;
                stateTime_ = 0.0f;
            }
            break;
        case ORANGE:
            if (stateTime_ >= 2.0f) {
                currentState_ = RED;
                stateTime_ = 0.0f;
            }
            break;
        case RED:
            if (stateTime_ >= 6.0f) {
                currentState_ = GREEN;
                stateTime_ = 0.0f;
            }
            break;
    }

}


// Renvoie la couleur à afficher pour chaque état
glm::vec3 TrafficLight::getColor(State state) const {
    switch (state) {
        case RED:    return glm::vec3(1.0f, 0.0f, 0.0f);
        case ORANGE: return glm::vec3(1.0f, 0.5f, 0.0f);
        case GREEN:  return glm::vec3(0.0f, 1.0f, 0.0f);
        default:     return glm::vec3(1.0f, 1.0f, 1.0f); // blanc si erreur
    }
}

// Dessin de l'ensemble du feu
void TrafficLight::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    GLuint shaderId = shader_->get_id();
    glUseProgram(shaderId);

    // Transmission des matrices pour le support
    GLint modelLoc = glGetUniformLocation(shaderId, "model");
    GLint viewLoc  = glGetUniformLocation(shaderId, "view");
    GLint projLoc  = glGetUniformLocation(shaderId, "projection");

    // 1. Dessiner le support (gris)
    glm::mat4 modelSupport = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
    modelSupport = glm::rotate(modelSupport, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelSupport[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
    GLint colorLoc = glGetUniformLocation(shaderId, "uColor");
    glUniform3fv(colorLoc, 1, &glm::vec3(0.3f, 0.3f, 0.3f)[0]);
    support_->draw(modelSupport, view, projection);

    // 2. Dessiner les 3 cubes (empilés sur le support)
    float startY = 2.3f;
    float offset = 1.0f;

    // Rouge (en haut)
    glm::mat4 modelRed = glm::translate(model, glm::vec3(0.0f, startY + offset * 2, 0.0f));
    glm::vec3 redColor = (currentState_ == RED) ? getColor(RED) : glm::vec3(0.2f, 0.0f, 0.0f);
    redCube_->setColor(redColor);
    redCube_->draw(modelRed, view, projection);

    // Orange (milieu)
    glm::mat4 modelOrange = glm::translate(model, glm::vec3(0.0f, startY + offset, 0.0f));
    glm::vec3 orangeColor = (currentState_ == ORANGE) ? getColor(ORANGE) : glm::vec3(0.1f, 0.05f, 0.0f);
    orangeCube_->setColor(orangeColor);
    orangeCube_->draw(modelOrange, view, projection);

    // Vert (bas)
    glm::mat4 modelGreen = glm::translate(model, glm::vec3(0.0f, startY, 0.0f));
    glm::vec3 greenColor = (currentState_ == GREEN) ? getColor(GREEN) : glm::vec3(0.0f, 0.1f, 0.0f);
    greenCube_->setColor(greenColor);
    greenCube_->draw(modelGreen, view, projection);
}
