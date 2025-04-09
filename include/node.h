#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.h"

class Shape;

class Node {
public:
    Node(const glm::mat4& transform = glm::mat4(1.0f));
    void add(Node* node);
    void add(Shape* shape);
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void key_handler(int key) const;
    void setTransform(const glm::mat4& transform) { this->transform_ = transform; }
    glm::mat4 getTransform() const { return transform_; }
    Shape* getShape() const { return (children_shape_.empty()) ? nullptr : children_shape_[0]; }
    
private:
    glm::mat4 transform_;
    std::vector<Node *> children_;
    std::vector<Shape *> children_shape_;
};
