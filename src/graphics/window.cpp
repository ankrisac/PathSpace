#include "core.hpp"
#include <GLFW/glfw3.h>

std::string debug_source(GLenum source) {
    switch(source) {
        case GL_DEBUG_SOURCE_API:             return "Api";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WindowManager";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "ShaderCompiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:     return "ThirdParty";
        case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
        case GL_DEBUG_SOURCE_OTHER:           return "Other";
        default:                              return "?";
    }
}
std::string debug_type(GLenum type) {
    switch(type) {
        case GL_DEBUG_TYPE_ERROR:               return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
        case GL_DEBUG_TYPE_PORTABILITY:         return "Error";
        case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
        case GL_DEBUG_TYPE_OTHER:               return "Other";
        case GL_DEBUG_TYPE_MARKER:              return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:          return "PushGroup";
        case GL_DEBUG_TYPE_POP_GROUP:           return "PopGroup";
        default:                                return "?";
    }
}
std::string debug_severity(GLenum severity) {
    switch(severity) {
        case GL_DEBUG_SEVERITY_HIGH:         return "High";
        case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
        case GL_DEBUG_SEVERITY_LOW:          return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "Note";
        default:                             return "?";
    }
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
                    GLsizei length, const GLchar *message, const void *userParam) {
    std::cerr 
        << debug_source(source)
        << "." << debug_type(type)
        << "." << debug_severity(severity)
        << ": "  << message << std::endl;

    if(severity == GL_DEBUG_SEVERITY_HIGH) std::abort();
}   

Window::Window(ivec2 size) {
    glfwInit();        

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    handle = glfwCreateWindow(size.x, size.y, "PathSpace", nullptr, nullptr);

    glfwMakeContextCurrent(handle);
    glDebugMessageCallback(debug_callback, nullptr);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}
Window::~Window() {
    glfwDestroyWindow(handle);
    glfwTerminate();
}

bool Window::should_close() {
    return glfwWindowShouldClose(handle);
}

void Window::update() {
    glfwGetCursorPos(handle, &mouse.pos.x, &mouse.pos.y);
    mouse.left.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_LEFT));
    mouse.middle.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_MIDDLE));
    mouse.right.update(glfwGetMouseButton(handle, GLFW_MOUSE_BUTTON_RIGHT));
    
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

dvec2 Window::get_mouse_pos() {
    f64 px, py;
    i32 width, height;
    glfwGetCursorPos(handle, &px, &py);
    glfwGetWindowSize(handle, &width, &height);

    return {
        2.0 * (px / width)  - 1.0,
        1.0 - 2.0f * (py / height)
    };
}
ivec2 Window::inner_size() {
    ivec2 size;
    glfwGetWindowSize(handle, &size.x, &size.y);
    return size;
}
