#include "core.hpp"

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

void Canvas::clear() {
    svg_data.clear();
    vertices.data.clear();
    indices.data.clear();
}
void Canvas::render() {
    vertices.upload(GL_STREAM_DRAW);
    indices.upload(GL_STREAM_DRAW);

    glUseProgram(program);
    glBindVertexArray(vertex_array);

    indices.bind();
    glDrawElements(GL_TRIANGLES, indices.data.size(), GL_UNSIGNED_INT, 0);      
}

void Canvas::fill_polygon(vec2 pos, f32 radius, u32 sides, Color color) {
    vec2 rotor  = vec2::polar(2.0f * M_PI / sides); 

    vertices.data.push_back({ pos, color });
    Index base = vertices.data.size();

    vec2 rad = { radius, 0.0f };
    for (u32 i = 0; i < sides; i++) {
        vertices.data.push_back({ pos + rad, color });

        indices.data.push_back(base - 1);
        indices.data.push_back(base + i);
        indices.data.push_back(base + (i + 1) % sides);

        rad = rad.rotate(rotor);
    }
}
void Canvas::stroke_polygon(vec2 pos, f32 radius, u32 sides, Color color) {
    vec2 rotor  = vec2::polar(2 * M_PI / sides);

    vec2 rad1 = { radius - 0.5f * style.width, 0.0f };
    vec2 rad2 = { radius + 0.5f * style.width, 0.0f };
    
    Index base = vertices.data.size();
    for(u32 i = 0; i < sides; i++) {
        vertices.data.push_back({ pos + rad1, color });
        vertices.data.push_back({ pos + rad2, color });

        for (auto idx : { 0, 2, 3, 0, 3, 1 }) {
            indices.data.push_back(base + (2 * i + idx) % (2 * sides));
        }

        rad1 = rad1.rotate(rotor);
        rad2 = rad2.rotate(rotor);
    }
}


std::ostream& operator<<(std::ostream& out, const Color color) {
    return out << "rgb("
        << color.r << ","
        << color.g << ","
        << color.b << ")";
}
std::ostream& operator<<(std::ostream& out, const Cap cap) {
    switch(cap) {
        case Cap::Butt:   return out << "butt";
        case Cap::Round:  return out << "round";
        case Cap::Square: return out << "square";
        default: return out << "unknown";
    }
}
std::ostream& operator<<(std::ostream& out, const Style style) {
    return out 
        << "fill='"         << style.fill   << "'"
        << "stroke='"       << style.stroke << "'"
        << "stroke-width='" << style.width  << "'";
}

void Canvas::circle(vec2 pos, f32 radius) {
    u32 sides = circle_sides(radius);
    if(!style.nofill) fill_polygon(pos, radius, sides, style.fill);
    if(!style.nostroke) stroke_polygon(pos, radius, sides, style.stroke);

    if(record) {
        svg_data << "  <circle";
        svg_data << " cx = '" << pos.x << "'";
        svg_data << " cy = '" << pos.y << "'";
        svg_data << " r = '" << std::abs(radius) << "\n";
        svg_data << "    " << style << "/>\n";
    }
}
void Canvas::line(vec2 a, vec2 b, Cap cap) {
    f32 radius = 0.5 * style.width;

    vec2 dir  = radius * (b - a).unit();
    vec2 perp = dir.perp(); 

    if(cap == Cap::Square) {
        a -= dir;
        b += dir;
    }

    Index base = vertices.data.size(); 
    vertices.data.push_back({ a + perp, style.stroke });
    vertices.data.push_back({ b + perp, style.stroke });
    vertices.data.push_back({ b - perp, style.stroke });
    vertices.data.push_back({ a - perp, style.stroke });

    for (auto idx : { 0, 1, 3, 1, 2, 3 }) {
        indices.data.push_back(base + idx);
    }

    u32 sides = circle_sides(radius);

    if(cap == Cap::Round) fill_polygon(a, radius, sides, style.stroke);
    if(cap == Cap::Round) fill_polygon(b, radius, sides, style.stroke);

    if(record) {
        svg_data << "  <line";
        svg_data << " x1='" << a.x << "'"; 
        svg_data << " x2='" << a.y << "'";
        svg_data << " y1='" << b.x << "'";
        svg_data << " y2='" << b.y << "'";
        svg_data << " stroke-linecap='" << cap << "'\n";
        svg_data << "    " << style << "/>\n";        
    }
}


void Canvas::arrow(vec2 pos, vec2 dir) {
    vec2 udir = 2 * style.width * dir.unit();
    vec2 perp = { -udir.y, udir.x };
    vec2 end = pos + dir;

    line(end, pos);
    line(end, end - udir + perp);
    line(end, end - udir - perp);
}
void Canvas::arrow_line(vec2 a, vec2 b) {
    vec2 dir = 2 * style.width * (b - a).unit();
    vec2 perp = { -dir.y, dir.x };
    vec2 c = 0.5f * (a + b);

    line(a, b);
    line(c, c - dir + perp);
    line(c, c - dir - perp);
}


void Canvas::save_as_svg(const std::string path) {
    std::fstream file;
    file.open(path, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error opening " << path << std::endl;
        std::abort();
    }

    file << "<svg viewBox='-1 -1 2 2' xmlns='http://www.w3.org/2000/svg'>\n";
    file << "  <g transform='scale(1,-1)'>\n";
    file << svg_data.rdbuf();
    file << "</svg>\n";
    file.close();
}