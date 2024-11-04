#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_texture;

out vec3 color;
out vec2 texture_pos;

void main() {
   color = vertex_color;
   gl_Position = vec4(vertex_position, 1.0);
   texture_pos = vertex_texture;
}