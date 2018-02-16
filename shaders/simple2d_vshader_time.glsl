#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec4 _color;

out vec4 _frag_color;

uniform float _time;

void main() {
  gl_Position = vec4(_pos.x, _pos.y, _pos.z, 1.0);
  _frag_color = vec4((1 + cos(2 * _time))/2.0, (1 + sin(2 * _time))/2.0, _color.ba);
}
