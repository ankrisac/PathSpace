#version 430

layout(location = 0) in  vec2 vert_uv;
layout(location = 0) out vec2 frag_uv;

void main() {
    gl_Position = vec4(2.0 * vert_uv - 1.0, 0.0, 1.0);
    frag_uv = vert_uv;
}