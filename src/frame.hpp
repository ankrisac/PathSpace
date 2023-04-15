#pragma once
#include "prelude.hpp"
#include "gfx.hpp"

template<typename T>
struct Grid2D {
    std::vector<T> data;
    Size size;

    Grid2D(Size size, T fill)
    : data(size.x * size.y, fill)
    , size(size) {} 

    bool contains(i32 i, i32 j) {
        return (0 <= i && i < size.x)
            && (0 <= j && j < size.y);
    }
    T& get(i32 i, i32 j) {
        return data[i + size.x * j];
    }
    const T& get(i32 i, i32 j) const {
        return data[i + size.x * j];
    }
};

struct Frame {
    GLuint m_program;
    GLuint m_vertex_array;
    GLuint m_buffer_vertex;
    GLuint m_texture;

    Frame();
    ~Frame();

    void render(const Grid2D<Color>& image);
};