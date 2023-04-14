#version 430

layout(binding = 0)  uniform sampler2D sampler;

layout(location = 0) in  vec2 frag_uv;

layout(location = 0) out vec4 diffuse;

void main() {
    diffuse = texture(sampler, frag_uv);
}