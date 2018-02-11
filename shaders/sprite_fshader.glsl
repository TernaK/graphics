#version 330 core

in vec2 _frag_tex_coord;

out vec4 _frag_color_out;

uniform sampler2D _texture0;

void main() {
  _frag_color_out = texture(_texture0, _frag_tex_coord);
}
