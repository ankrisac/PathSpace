#pragma once
#include "prelude.hpp"
#include "gfx.hpp"

#include <ostream>
#include <fstream>

struct Frame {
    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_buffer_vertex;
    GLuint m_texture;

    Frame();
    ~Frame();

    void render(const Grid2D<Color>& image);
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
    Color stroke = { 0.0, 0.0, 0.0, 1.0 };
    f32 width = 0.1;

    std::string to_string() {
        return fmt::format(
            "fill='{}' stroke='{}' stroke-width='{}'",
            color_to_string(fill), color_to_string(stroke), width
        );
    }
private:
    static std::string color_to_string(Color val) {
        return fmt::format(
            "rgba({}, {}, {}, {})", 
            255 * val.r, 255 * val.g, 255 * val.b, val.a
        );
    }
};

// Workaround for compiler bug
template<typename T> T Default() {
    return {};
}

enum class Cap { Butt, Round, Square };
class ICanvas {
public:
    Style style;

    virtual void line(Vec2 a, Vec2 b, Cap cap = Cap::Round) = 0;

    struct CircleArgs {
        bool fill   = true;
        bool stroke = true;
    };
    virtual void circle(Vec2 pos, f32 radius, CircleArgs args = Default<CircleArgs>()) = 0;

    void arrow(Vec2 pos, Vec2 dir);
};

class Canvas : public ICanvas {
public:
    Canvas();
    ~Canvas();

    Canvas(const Canvas&) = delete;
    Canvas& operator=(const Canvas&) = delete;

    void line(Vec2 a, Vec2 b, Cap cap = Cap::Round) override;
    void circle(Vec2 pos, f32 radius, CircleArgs args = Default<CircleArgs>()) override;

    void render();
private:
    void polygon_interior(Vec2 pos, f32 radius, u32 sides, Color color);
    void polygon_boundary(Vec2 pos, f32 radius, u32 sides, Color color);
    u32 circle_sides(f32 radius) {
        return 64;    
    }

    Buffer<GL_ARRAY_BUFFER, Vertex>      vertices;
    Buffer<GL_ELEMENT_ARRAY_BUFFER, u32> indices;

    GLuint program;
    GLuint vertex_array;
};

class SvgCanvas : public ICanvas {
public:
    SvgCanvas() {}
    ~SvgCanvas() {}

    void line(Vec2 a, Vec2 b, Cap cap = Cap::Round) override;
    void circle(Vec2 pos, f32 radius, CircleArgs args = Default<CircleArgs>()) override;
    void save(const std::string path);
private:
    std::vector<std::string> primitives;
};
