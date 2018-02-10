#version 330 core

in vec4 _frag_color;
in vec3 _frag_pos;
in vec3 _frag_normal;

out vec4 _frag_color_out;

uniform vec3 _cam_pos;
uniform struct Light {
  float ph_exp;
  vec3 position;
  vec3 ambient;
  vec3 color;
} _light;

void main() {
  vec3 l = normalize(_light.position - _frag_pos);
  float cos_t = clamp(dot(_frag_normal, l), 0, 1);
  vec3 diffuse = cos_t * _light.color;

  vec3 c = normalize(_cam_pos - _frag_pos);
  float l_reflected_on_c = dot(normalize(reflect(-l, _frag_normal)), c);
  float spec = pow( max(l_reflected_on_c, 0.0), _light.ph_exp );
  vec3 specular = spec * _light.color;
  _frag_color_out = vec4((diffuse + specular + _light.ambient) * _frag_color.xyz,
                         _frag_color.a);
}
