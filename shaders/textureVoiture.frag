// texture.frag
#version 330 core

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_frag_pos;

out vec4 FragColor;

uniform sampler2D diffuse_map;
uniform vec3 light_dir;
uniform vec3 light_color;
uniform vec3 view_pos; // position de la caméra

void main() {
    vec3 texture_color = texture(diffuse_map, v_texcoord).rgb;

    vec3 normal = normalize(v_normal);


    vec3 lightDir = normalize(-light_dir);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light_color;

    vec3 viewDir = normalize(view_pos - v_frag_pos);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * light_color;

    // Ambiante
    vec3 ambient = 0.1 * light_color;

    // Résultat final
    vec3 result = (ambient + diffuse + specular) * texture_color;
    FragColor = vec4(result, 1.0);
}
