#pragma once
#include "prelude.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

struct Mouse {
    Vec2 pos;
    bool pressed;
    bool released;
};

struct Window {
    GLFWwindow* handle;

    Window(Size size);    
    ~Window();

    bool should_close();
    void update();
};

GLuint load_shader(std::string path, GLenum type);
GLuint load_program(std::string path);

template<GLenum TARGET, typename T>
struct Buffer {
    std::vector<T> data; 
    GLuint handle;

    Buffer() { glGenBuffers(1, &handle); }
    ~Buffer() { glGenBuffers(1, &handle); }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void bind() {
        glBindBuffer(TARGET, handle);
    }
    void upload(GLenum usage) {
        glBindBuffer(TARGET, handle);
        glBufferData(TARGET, data.size() * sizeof(T), data.data(), usage);
    }
};