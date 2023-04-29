#include "core.hpp"

Frame::Frame(ivec2 size)
: size(size)
, m_weight(size, Float(0))
, m_average(size, Colors::BLACK) 
, m_final(size, { 0.0, 0.0, 0.0 })
{
    m_program = load_program("shaders/frame");

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    glGenBuffers(1, &m_buffer_vertex);        

    vec2 uv[] = {
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

void Frame::render() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, 
        size.x, size.y, 0,
        GL_RGB, GL_FLOAT, m_final.data.data()    
    );

    glUseProgram(m_program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBindVertexArray(m_vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Frame::add_bilinear(Vec2f uv, Color value) {
    Float px, py;
    Float dx = std::modf(uv.x * static_cast<Float>(size.y), &px);
    Float dy = std::modf(uv.y * static_cast<Float>(size.y), &py);
    
    i32 pi = static_cast<i32>(px);
    i32 pj = static_cast<i32>(py);
    Float wx[] = { dx, Float(1) - dx };
    Float wy[] = { dy, Float(1) - dy };

    for(i32 j = 0; j < 2; j++) {
        for(i32 i = 0; i < 2; i++) {
            add_sample(pi + i, pj + j, value, wx[i] * wy[j]);
        }
    }
}
void Frame::add_sample(i32 i, i32 j, Color value, Float weight) {
    if(0 <= i && i < size.x && 0 <= j && j < size.y) {
        Float W = m_weight.get(i, j);

        m_weight.get(i, j) += weight;
        m_average.get(i, j) += weight * (value - m_average.get(i, j)) / (W + weight);
        m_final.get(i, j) = RGB::from(m_average.get(i, j));
    }
}
