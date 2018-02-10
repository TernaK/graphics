#version 330 core

in vec4 _frag_color;

out vec4 _frag_color_out;

void main() {
  _frag_color_out = _frag_color;
}
