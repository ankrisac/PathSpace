#version 430

layout(location = 0) in  vec4 frag_color;
layout(location = 0) out vec4 diffuse;

void main() {
    diffuse = frag_color;
}
