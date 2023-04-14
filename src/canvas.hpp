#pragma once

#include "prelude.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

struct Window {
    GLFWwindow* m_handle;

    Window(Size size);    
    ~Window();

    bool should_close();
    void update();
};

struct Frame {
    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_vertex_buffer;
    GLuint m_texture;

    Frame();
    ~Frame();

    void draw(const Grid2D<Color>& image);
};