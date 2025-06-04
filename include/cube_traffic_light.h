#ifndef CUBE_TRAFFIC_LIGHT_H
#define CUBE_TRAFFIC_LIGHT_H

#include "cubesimple.h"
#include <glm/glm.hpp>

class CubeTrafficLight : public CubeSimple {
public:
    CubeTrafficLight(Shader* shader, const glm::vec3& size = glm::vec3(1.0f));
    ~CubeTrafficLight() = default;

    void setColor(const glm::vec3& color);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;

private:
    glm::vec3 dynamic_color_;
    GLuint VAO;
};

#endif // CUBE_TRAFFIC_LIGHT_H
