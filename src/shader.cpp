#include "shader.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path) {
    GLuint vert_shader, frag_shader;
    GLint status;

    // Create vertex shader
    vert_shader = compile_shader(vertex_path, GL_VERTEX_SHADER);

    // Create fragment shader
    frag_shader = compile_shader(fragment_path, GL_FRAGMENT_SHADER);

    // Create shader program
    glid = glCreateProgram();
    glAttachShader(glid, vert_shader);
    glAttachShader(glid, frag_shader);
    glLinkProgram(glid);

    // Check if shader program linked successfully
    glGetProgramiv(glid, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetProgramInfoLog(glid, 512, NULL, buffer);
        cerr << "Failed to link shader program: " << buffer << endl;
        exit(EXIT_FAILURE);
    }

    // Delete shaders
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

Shader::~Shader() {
    glUseProgram(0);
    glDeleteProgram(glid);
}

GLuint Shader::get_id() {
    return glid;
}

GLuint Shader::compile_shader(const std::string& path, GLenum shader_type) {

    string source;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Failed to open shader file: " << path << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    while (getline(file, line)) {
        source += line + "\n";
    }

    file.close();
    
    GLuint shader;
    GLint status;
    const GLchar* src_array[1] = {source.c_str()};

    // Create shader
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, src_array, NULL);
    glCompileShader(shader);

    // Check if shader compiled successfully
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        cerr << "Failed to compile shader: " << buffer << endl;
        exit(EXIT_FAILURE);
    }

    return shader;
}
