#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec3 color;

uniform float scale;
uniform vec3 addVec;

void main() {
   color = vertex_color;
   gl_Position = vec4((vertex_position + addVec) * scale, 1.0);
}