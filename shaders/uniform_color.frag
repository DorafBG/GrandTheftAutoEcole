// uniform_color.frag
#version 330 core
uniform vec3 uColor;
out vec4 out_color;
void main() {
    out_color = vec4(uColor, 1.0);
}
