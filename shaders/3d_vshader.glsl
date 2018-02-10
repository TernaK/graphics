#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec4 _color;

out vec4 _frag_color;
out vec3 _frag_pos;

uniform mat4 _proj_mat;
uniform mat4 _view_mat;
uniform mat4 _model_mat;

void main() {
  vec4 pos = _model_mat * vec4(_pos, 1.0);
  gl_Position = _proj_mat * _view_mat * pos;
  _frag_pos = pos.xyz;
  _frag_color = _color;
}
