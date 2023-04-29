#include "core.hpp"

GLuint load_shader(std::string path, GLenum type) {
    GLuint handle = glCreateShader(type);
    glObjectLabel(GL_SHADER, handle, path.size(), path.c_str());

    std::string src = read_file(path);
    const GLchar* source = static_cast<const GLchar*>(src.c_str());
    glShaderSource(handle, 1, &source, 0);
    glCompileShader(handle);

    return handle;
}
GLuint load_program(std::string path) {
    GLuint program = glCreateProgram();
    glObjectLabel(GL_PROGRAM, program, path.size(), path.c_str());

    GLuint vert = load_shader(path + ".vert", GL_VERTEX_SHADER);
    GLuint frag = load_shader(path + ".frag", GL_FRAGMENT_SHADER);

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vert);
    glDetachShader(program, frag);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}
