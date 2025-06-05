#version 330 core

in vec3 FragNormal;
in vec3 FragPos;

uniform vec3 uColor;

out vec4 out_color;

void main() {
    // Direction de la lumière (ex : venant du dessus)
    vec3 lightDir = normalize(vec3(0.0, 1.0, 0.0));

    // Normalisation de la normale interpolée
    float diff = max(dot(normalize(FragNormal), lightDir), 0.7);; // 0.2 = lumière ambiante minimale
    vec3 color = uColor * diff;

    out_color = vec4(color, 1.0);
}
