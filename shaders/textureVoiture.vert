#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_frag_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;

    v_texcoord = texcoord;
    v_frag_pos = vec3(world_position);

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    v_normal = normalMatrix * normal;
}
