#ifndef BANC_H
#define BANC_H

#include "shape.h"
#include "cylinder.h"
#include "cubesimple.h"
#include <vector>
#include <glm/glm.hpp>

class Banc : public Shape {
public:
    Banc(Shader* shader, const glm::vec3& color = glm::vec3(0.6f, 0.3f, 0.1f));
    virtual ~Banc();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    glm::vec3 color_;
    std::vector<Cylinder*> pieds_;    // 4 pieds cylindriques
    CubeSimple* assise_;              // Assise (cube)
    CubeSimple* dossier_;             // Dossier (cube)
};

#endif // BANC_H
