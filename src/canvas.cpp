#include "canvas.hpp"

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

    m_handle = glfwCreateWindow(size.x, size.y, "PathSpace", nullptr, nullptr);

    glfwMakeContextCurrent(m_handle);
    glDebugMessageCallback(debug_callback, nullptr);
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
        fmt::print("Error: Cannot open {}\n", path);
    }
 
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

GLuint load_shader(std::string path, GLenum type) {
    GLuint handle = glCreateShader(type);
    glObjectLabel(GL_SHADER, handle, path.length(), path.c_str());

    std::string src = read_file(path);    
    const GLchar* source[1] = { static_cast<const GLchar*>(src.c_str()) };
    const GLsizei length[1] = { static_cast<GLsizei>(src.length()) };

    glShaderSource(handle, 1, source, length);
    glCompileShader(handle);

    return handle;
}
GLuint load_program(std::string path) {
    GLuint program = glCreateProgram();
    glObjectLabel(GL_PROGRAM, program, path.length(), path.c_str());

    GLuint vert = load_shader(path + ".vert", GL_VERTEX_SHADER);
    GLuint frag = load_shader(path + ".frag", GL_FRAGMENT_SHADER);

    glAttachShader(program, vert);
    glAttachShader(program, frag);

    glLinkProgram(program);

    glDeleteShader(vert);
    glDeleteShader(frag);

    return program;
}

Frame::Frame() {
    m_program = load_program("shaders/frame");

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_vertex_buffer);        

    Vec2 vertices[] = {
        {  1.0,  1.0 },
        { -1.0,  1.0 },
        { -1.0, -1.0 },
        {  1.0,  1.0 },
        { -1.0, -1.0 },
        {  1.0, -1.0 }
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
Frame::~Frame() {
    glDeleteTextures(1, &m_texture);
    
    glDeleteBuffers(1, &m_vertex_buffer);
    glDeleteVertexArrays(1, &m_vertex_array);
    
    glDeleteProgram(m_program);
}

void Frame::draw(const Grid2D<Color>& image) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, 
        image.size.x, image.size.y, 0,
        GL_RGBA, GL_FLOAT, image.data.data()    
    );

    glUseProgram(m_program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray(m_vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}