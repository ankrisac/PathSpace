#version 430

layout(location = 0) in  vec2 vertex_pos;
layout(location = 0) out vec2 frag_uv;

void main() {
    gl_Position = vec4(vertex_pos, 0.0, 2.0);
    frag_uv = vertex_pos;
}