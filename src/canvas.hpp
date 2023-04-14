#pragma once
#include "prelude.hpp"
#include "gfx.hpp"

struct Frame {
    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_buffer_vertex;
    GLuint m_texture;

    Frame();
    ~Frame();

    void draw(const Grid2D<Color>& image);
};

struct Vertex {
    Vec2 pos;
    Color color;

    static void set_layout() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)sizeof(Vec2));
    } 
};

struct Style {
    Color fill = { 1.0, 1.0, 1.0, 1.0 };
    Color stroke = { 0.0, 0.0, 0.0, 0.0 };
    f32 width = 0.01;
};

class Canvas {
public:
    Style style;

    Canvas();
    ~Canvas();

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    void draw();

    struct LineArgs { 
        bool cap_a = true; 
        bool cap_b = true; 
    };
    void line(Vec2 a, Vec2 b, LineArgs args = {true, true});
    void arrow(Vec2 pos, Vec2 dir);

    struct CircleArgs { 
        bool fill = true; 
        bool stroke = true; 
    };
    void circle(Vec2 pos, f32 radius, CircleArgs args = {true, true});

    
private:
    void polygon_interior(Vec2 pos, f32 radius, u32 sides, Color color);
    void polygon_boundary(Vec2 pos, f32 radius, u32 sides, Color color);
    u32 circle_sides(f32 radius) {
        return 256;    
    }

    GLuint program;
    GLuint vertex_array;

    Buffer<GL_ARRAY_BUFFER, Vertex>      vertices;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, u32> indices;
};