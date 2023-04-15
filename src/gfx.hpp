#pragma once
#include "prelude.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

struct Button {
    bool prev;
    bool curr;

    void update(int state) {
        prev = curr;
        curr = (state == GLFW_PRESS);
    }

    bool pressed() { return curr && !prev; }
    bool released() { return !curr && prev; }
};
struct Mouse {
    Vec2 pos;
    Button left;
    Button middle;
    Button right;

    bool press() {
        return left.curr 
            || middle.curr 
            || right.curr;
    }
    bool pressed() {
        return left.pressed() 
            || middle.pressed() 
            || right.pressed();
    }
    bool released() {
        return left.released() 
            || middle.released() 
            || right.released();
    }
};

struct Window {
    GLFWwindow* handle;
    Mouse mouse;

    Window(Size size);    
    ~Window();

    Vec2 get_mouse_pos() {
        f64 px, py;
        i32 width, height;
        glfwGetCursorPos(handle, &px, &py);
        glfwGetWindowSize(handle, &width, &height);

        return {
            2.0f * (px / width)  - 1.0f,
            1.0f - 2.0f * (py / height)
        };
    }

    bool should_close() {
        return glfwWindowShouldClose(handle);
    }
    void update() {
        f64 x, y;
        glfwGetCursorPos(handle, &x, &y);
        mouse.pos = { x, y };

        mouse.left.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT));
        mouse.middle.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_MIDDLE));
        mouse.right.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_RIGHT));
        
        glfwSwapBuffers(handle);
        glfwPollEvents();
    }
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