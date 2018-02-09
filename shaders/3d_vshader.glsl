#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec4 _color;

out vec4 _frag_color;

uniform mat4 _proj_mat;
uniform mat4 _view_mat;
uniform mat4 _model_mat;

void main() {
  gl_Position = _proj_mat * _view_mat * _model_mat * vec4(_pos, 1.0);
  _frag_color = _color;
}
