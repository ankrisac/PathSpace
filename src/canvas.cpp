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

Frame::Frame() {
    m_program = load_program("shaders/frame");

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_buffer_vertex);        

    Vec2 uv[] = {
        { 1.0, 1.0 },
        { 0.0, 1.0 },
        { 0.0, 0.0 },
        { 1.0, 1.0 },
        { 0.0, 0.0 },
        { 1.0, 0.0 }
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), &uv, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
Frame::~Frame() {
    glDeleteTextures(1, &m_texture);
    
    glDeleteBuffers(1, &m_buffer_vertex);
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



Canvas::Canvas() {
    program = load_program("shaders/canvas");

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    vertices.bind();
    Vertex::set_layout();
}
Canvas::~Canvas() {
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(program);
}

void Canvas::draw() {
    vertices.upload(GL_STREAM_DRAW);
    indices.upload(GL_STREAM_DRAW);

    glUseProgram(program);
    glBindVertexArray(vertex_array);

    indices.bind();
    glDrawElements(GL_TRIANGLES, indices.data.size(), GL_UNSIGNED_INT, 0);      

    vertices.data.clear();
    indices.data.clear();
}

void Canvas::line(Vec2 a, Vec2 b, bool cap) {
    Vec2 dir  = glm::normalize(b - a);
    Vec2 perp = style.width * Vec2(-dir.y, dir.x); 

    u32 base = vertices.data.size(); 
    vertices.data.push_back({ a + perp, style.stroke });
    vertices.data.push_back({ b + perp, style.stroke });
    vertices.data.push_back({ b - perp, style.stroke });
    vertices.data.push_back({ a - perp, style.stroke });

    for (auto idx : { 0, 1, 3, 1, 2, 3 }) {
        indices.data.push_back(base + idx);
    }

    if(cap) {
        f32 radius = style.width;
        u32 sides = circle_sides(radius);
        polygon_interior(a, radius, sides, style.stroke);
        polygon_interior(b, radius, sides, style.stroke);
    }
}

Vec2 complex_mul(Vec2 a, Vec2 b) {
    return {
        a.x * b.x - a.y * b.y,
        a.x * b.y + a.y * b.x
    };
}
void Canvas::polygon_interior(Vec2 pos, f32 radius, u32 sides, Color color) {
    f32 angle   = 2 * PI / sides;
    Vec2 rotor  = Vec2(std::cos(angle), std::sin(angle));

    vertices.data.push_back({ pos, color });
    u32 base = vertices.data.size();

    Vec2 rad = Vec2(radius, 0.0);
    for (u32 i = 0; i < sides; i++) {
        vertices.data.push_back({ pos + rad, color });

        indices.data.push_back(base - 1);
        indices.data.push_back(base + i);
        indices.data.push_back(base + (i + 1) % sides);

        rad = complex_mul(rad, rotor);
    }
}
void Canvas::polygon_boundary(Vec2 pos, f32 radius, u32 sides, Color color) {
    f32 angle   = 2 * PI / sides;
    Vec2 rotor  = Vec2(std::cos(angle), std::sin(angle));

    Vec2 rad1 = Vec2(radius - style.width, 0.0);
    Vec2 rad2 = Vec2(radius + style.width, 0.0);
    
    u32 base = vertices.data.size();
    for(u32 i = 0; i < sides; i++) {
        vertices.data.push_back({ pos + rad1, color });
        vertices.data.push_back({ pos + rad2, color });

        for (auto idx : { 0, 2, 3, 0, 3, 1 }) {
            indices.data.push_back(base + (2 * i + idx) % (2 * sides));
        }

        rad1 = complex_mul(rad1, rotor);
        rad2 = complex_mul(rad2, rotor);
    }
}


void Canvas::circle(Vec2 pos, f32 radius, bool fill, bool stroke) {
    u32 sides = circle_sides(radius);
    if(fill)   polygon_interior(pos, radius, sides, style.fill);
    if(stroke) polygon_boundary(pos, radius, sides, style.stroke);
}