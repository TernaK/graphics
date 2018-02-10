#version 330 core

in vec4 _frag_color;
in vec3 _frag_pos;

out vec4 _frag_color_out;

uniform struct Light {
  vec3 position;
  vec3 ambient;
  vec3 color;
} _light;

void main() {
  vec3 l = normalize(_light.position - _frag_pos);
  _frag_color_out = _frag_color;
}
