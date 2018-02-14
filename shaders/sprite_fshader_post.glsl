#version 330 core

in vec2 _frag_tex_coord;

out vec4 _frag_color_out;

uniform sampler2D _texture0;

void main() {
  vec4 px = texture(_texture0, _frag_tex_coord);
  vec4 px1 = texture(_texture0, _frag_tex_coord + vec2(0.004));
  vec4 color = abs(px - px1);
  _frag_color_out = color;
}
