#ifndef EOLIENNE_H
#define EOLIENNE_H

#include "shape.h"
#include "cylinder.h"
#include <vector>
#include <glm/glm.hpp>

class Eolienne : public Shape {
public:
    Eolienne(Shader* shader, const glm::vec3& color = glm::vec3(1.0f)); // blanc par défaut
    ~Eolienne();

    void update(float deltaTime); // deltaTime : temps écoulé depuis la dernière frame, en secondes

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    Cylinder* support_;                // le "mât"
    Cylinder* axe_;                    // axe horizontal
    std::vector<Cylinder*> pales_;     // 3 pales
    glm::vec3 color_;
    float angle_pales_ = 0.0f; // angle en degrés ou radians

};

#endif // EOLIENNE_H
