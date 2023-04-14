#include "gfx.hpp"

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
    fmt::print("{}.{}.{}: {}\n", 
        debug_source(source),
        debug_type(type),
        debug_severity(severity),
        message
    );

    if(severity == GL_DEBUG_SEVERITY_HIGH) std::abort();
}   

Window::Window(Size size) {
    glfwInit();        

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    m_handle = glfwCreateWindow(size.x, size.y, "PathSpace", nullptr, nullptr);

    glfwMakeContextCurrent(m_handle);
    glDebugMessageCallback(debug_callback, nullptr);

    glEnable(GL_MULTISAMPLE);
}
Window::~Window() {
    glfwDestroyWindow(m_handle);
    glfwTerminate();
}

bool Window::should_close() {
    return glfwWindowShouldClose(m_handle);
}
void Window::update() {
    glfwSwapBuffers(m_handle);
    glfwPollEvents();
}



#include <fstream>
#include <sstream>

std::string read_file(std::string path) {
    std::fstream file(path);

    if (!file.is_open()) {
        fmt::print("Error: Cannot open file [{}]\n", path);
        std::abort();
    }
 
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

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
