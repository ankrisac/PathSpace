#include "canvas.hpp"



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

void ICanvas::arrow(Vec2 pos, Vec2 dir) {
    Vec2 udir = 3 * style.width * glm::normalize(dir);
    Vec2 perp = { -udir.y, udir.x };
    Vec2 end = pos + dir;

    line(end, pos);
    line(end, end - udir + perp);
    line(end, end - udir - perp);
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

void Canvas::render() {
    vertices.upload(GL_STREAM_DRAW);
    indices.upload(GL_STREAM_DRAW);

    glUseProgram(program);
    glBindVertexArray(vertex_array);

    indices.bind();
    glDrawElements(GL_TRIANGLES, indices.data.size(), GL_UNSIGNED_INT, 0);      

    vertices.data.clear();
    indices.data.clear();
}

void Canvas::line(Vec2 a, Vec2 b, Cap cap) {
    f32 radius = 0.5 * style.width;

    Vec2 dir  = radius * glm::normalize(b - a);
    Vec2 perp = Vec2(-dir.y, dir.x); 

    if(cap == Cap::Square) {
        a -= dir;
        b += dir;
    }

    u32 base = vertices.data.size(); 
    vertices.data.push_back({ a + perp, style.stroke });
    vertices.data.push_back({ b + perp, style.stroke });
    vertices.data.push_back({ b - perp, style.stroke });
    vertices.data.push_back({ a - perp, style.stroke });

    for (auto idx : { 0, 1, 3, 1, 2, 3 }) {
        indices.data.push_back(base + idx);
    }

    u32 sides = circle_sides(radius);

    if(cap == Cap::Round) polygon_interior(a, radius, sides, style.stroke);
    if(cap == Cap::Round) polygon_interior(b, radius, sides, style.stroke);
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

    Vec2 rad1 = Vec2(radius - 0.5 * style.width, 0.0);
    Vec2 rad2 = Vec2(radius + 0.5 * style.width, 0.0);
    
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

void Canvas::circle(Vec2 pos, f32 radius, CircleArgs args) {
    u32 sides = circle_sides(radius);
    if(args.fill)   polygon_interior(pos, radius, sides, style.fill);
    if(args.stroke) polygon_boundary(pos, radius, sides, style.stroke);
}


constexpr const char* linecap_string(Cap cap) {
    switch(cap) {
        case Cap::Butt:   return "butt";
        case Cap::Round:  return "round";
        case Cap::Square: return "square";
        default: return "";
    }
}
void SvgCanvas::line(Vec2 a, Vec2 b, Cap cap) { 
    primitives.push_back(fmt::format(
        "  <line x1='{}' y1='{}' x2='{}' y2='{}'\n"
        "    {} stroke-linecap='{}'/>\n",
        a.x, a.y, b.x, b.y, style.to_string(), linecap_string(cap)
    ));
}
void SvgCanvas::circle(Vec2 pos, f32 radius, CircleArgs args) {
    primitives.push_back(fmt::format(
        "  <circle cx='{}' cy='{}' r='{}'\n"
        "    {}/>\n",
        pos.x, pos.y, std::abs(radius), style.to_string()
    ));
}

void SvgCanvas::save(const std::string path) {
    std::fstream file;
    file.open("out/example.svg", std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        fmt::print("Error opening {}\n", "out/example.svg");
        std::abort();
    }

    file << "<svg viewBox='-1 -1 2 2' xmlns='http://www.w3.org/2000/svg'>\n";
    file << "  <g transform='scale(1,-1)'>\n";
    for (auto& p : primitives) {
        file << p;
    }
    file << "</svg>\n";
    file.close();
}