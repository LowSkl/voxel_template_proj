#version 460

in vec3 color;
in vec2 texture_pos;

out vec4 frag_color;

uniform sampler2D tex_slot;

void main() {
   frag_color = texture(tex_slot, texture_pos) * vec4(color, 1.f);
}