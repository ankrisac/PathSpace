#pragma once

#include "../prelude.hpp"
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>



GLuint load_shader(std::string path, GLenum type);
GLuint load_program(std::string path);

template<GLenum TARGET, typename T>
struct Buffer {
    std::vector<T> data; 
    GLuint handle;

    Buffer() { 
        glGenBuffers(1, &handle); 
    }
    ~Buffer() { 
        glGenBuffers(1, &handle); 
    }

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
    dvec2 pos;
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

    Window(ivec2 size);
    ~Window();

    bool should_close();
    void update();

    dvec2 get_mouse_pos();
    ivec2 inner_size();
};





struct Style {
    Color fill = { 1.0, 1.0, 1.0 };
    Color stroke = { 0.0, 0.0, 0.0 };

    bool nofill = false;
    bool nostroke = false;
    f32 width = 0.1;
};

enum class Cap { Butt, Round, Square };
class Canvas {
public:
    Style style;
    bool record = false;

    Canvas();
    ~Canvas();

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    void clear();
    void render();
    void save_as_svg(const std::string path);

    template<typename Fn> 
    void with(Fn fn) {
        Style old = style;
        fn();
        style = old;
    }

    void line(vec2 a, vec2 b, Cap cap = Cap::Round);
    void circle(vec2 pos, f32 radius);

    void arrow(vec2 pos, vec2 dir);
    void arrow_line(vec2 a, vec2 b);
private:      
    using Index = u32;
    struct Vertex {
        vec2 pos;
        Color color;

        static void set_layout() {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), 0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)sizeof(vec2));
        } 
    };
    
    void fill_polygon(vec2 pos, f32 radius, u32 sides, Color color);
    void stroke_polygon(vec2 pos, f32 radius, u32 sides, Color color);

    u32 circle_sides(f32 radius) {
        return 64 + 256 * radius;    
    }

    std::stringstream svg_data;

    Buffer<GL_ARRAY_BUFFER, Vertex>      vertices;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, Index> indices;

    GLuint program;
    GLuint vertex_array;
};





template<typename T>
struct Grid2D {
    std::vector<T> data;
    ivec2 size;

    Grid2D(ivec2 size, T fill)
    : data(size.x * size.y, fill)
    , size(size)
    {} 

    T& get(i32 i, i32 j) {
        return data[i + size.x * j];
    }
    const T& get(i32 i, i32 j) const {
        return data[i + size.x * j];
    }
};

struct RGB {
    f32 r;
    f32 g;
    f32 b;

    static RGB from(Color color) {
        return { 
            static_cast<f32>(color.r), 
            static_cast<f32>(color.g), 
            static_cast<f32>(color.b) 
        };
    }
};
struct Frame {
    Frame(ivec2 size);
    ~Frame();

    void render();
    void reset() {
        for(size_t i = 0; i < m_weight.data.size(); i++) {
            m_weight.data[i] = 0.0;
        }
    }

    ivec2 get_size() const {
        return size;
    }
    void add_sample(i32 i, i32 j, Color color, Float weight);
    void add_bilinear(Vec2f pixel, Color color);
private:
    ivec2 size;
    Grid2D<Float> m_weight;
    Grid2D<Color> m_average;
    Grid2D<RGB>   m_final;

    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_buffer_vertex;
    GLuint m_texture;
};
