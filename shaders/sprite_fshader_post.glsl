#version 330 core

in vec2 _frag_tex_coord;

out vec4 _frag_color_out;

uniform sampler2D _texture0;
uniform float _time;
uniform float _width;
uniform float _height;

void main() {
  float x = _time + _width * _frag_tex_coord.x;
  float y = _time + _height * _frag_tex_coord.y;
  vec4 px = texture(_texture0, _frag_tex_coord + 0.02 * vec2(sin(x),cos(y)));
  _frag_color_out = px;
}
