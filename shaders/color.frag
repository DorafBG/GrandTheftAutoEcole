#version 330 core

in vec3 fragColor;

// output fragment color for OpenGL
out vec4 out_color;

void main() {
    out_color = vec4(fragColor, 1);
}
