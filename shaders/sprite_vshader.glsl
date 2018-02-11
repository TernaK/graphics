#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec2 _tex_coord;

out vec2 _frag_tex_coord;

void main() {
  gl_Position = vec4(_pos, 1.0);
  _frag_tex_coord = _tex_coord;
}
