#version 330 core

in vec2 v_texcoord;
out vec4 FragColor;

uniform sampler2D diffuse_map;

void main() {
    FragColor = texture(diffuse_map, v_texcoord);
}
