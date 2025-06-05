#ifndef ARBRE_H
#define ARBRE_H

#include "shape.h"
#include "cylinder.h"
#include "cubesimple.h"
#include <glm/glm.hpp>

class Arbre : public Shape {
public:
    Arbre(Shader* shader);
    virtual ~Arbre();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void setPosition(const glm::vec3& pos) { position_ = pos; }


private:
    Cylinder* tronc_;
    CubeSimple* feuilles_;
    glm::vec3 position_;
};

#endif // ARBRE_H
