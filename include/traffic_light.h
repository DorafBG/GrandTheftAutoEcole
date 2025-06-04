#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "shape.h"
#include "cylinder.h"
#include "cube_traffic_light.h"
#include "shader.h"
#include <glm/glm.hpp>

class TrafficLight : public Shape {
public:
    enum State { RED, ORANGE, GREEN };

    TrafficLight(Shader* shader);
    ~TrafficLight();

    void update(float deltaTime); // deltaTime en secondes
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

    State getState() const { return currentState_; }

private:
    // État du feu et timer interne
    State currentState_;
    float stateTime_;


    // Composants graphiques
    Cylinder* support_;
    CubeTrafficLight* redCube_;
    CubeTrafficLight* orangeCube_;
    CubeTrafficLight* greenCube_;


    // Pointeur vers le shader partagé
    Shader* shader_;

    // Méthode utilitaire pour obtenir la couleur du feu selon l'état
    glm::vec3 getColor(State state) const;
};

#endif // TRAFFIC_LIGHT_H
