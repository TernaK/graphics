#version 330 core

in vec3 _frag_pos;
in vec3 _frag_normal;

out vec4 _frag_color_out;

uniform vec3 _cam_pos;
uniform struct Light {
  vec3 direction;
  vec3 ambient;
  vec3 color;
} _light;
uniform struct Material {
  vec3 color;
  float shininess;
  float alpha;
} _material;

void main() {
  vec3 l = normalize(-_light.direction);
  float cos_t = clamp(dot(l, _frag_normal), 0, 1);
  vec3 diffuse = cos_t * _light.color;

  vec3 c = normalize(_cam_pos - _frag_pos);
  float l_reflected_on_c = dot(normalize(reflect(-l, _frag_normal)), c);
  float spec = pow( max(l_reflected_on_c, 0.0), _material.shininess );
  vec3 specular = spec * _light.color;

  _frag_color_out = vec4((diffuse + specular + _light.ambient) * _material.color,
                         _material.alpha);
}
