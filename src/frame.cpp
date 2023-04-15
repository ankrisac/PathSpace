#include "frame.hpp"

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

void Frame::render(const Grid2D<Color>& image) {
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
