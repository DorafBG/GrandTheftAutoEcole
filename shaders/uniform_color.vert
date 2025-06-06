#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal; // normal transformée
out vec3 FragPos;    // position du fragment en espace monde

void main() {
    FragPos = vec3(model * vec4(position, 1.0));
    FragNormal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
