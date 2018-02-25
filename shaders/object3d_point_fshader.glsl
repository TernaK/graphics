#version 330 core

in vec3 _frag_pos;
in vec3 _frag_normal;

out vec4 _frag_color_out;

uniform vec3 _cam_pos;
uniform struct Light {
  vec3 position;
  vec3 ambient;
  vec3 color;
  vec3 attenuation;
  vec3 strength;
} _light;
uniform struct Material {
  vec3 color;
  float shininess;
  float alpha;
} _material;
int _num_lights = 1;

void add_light_contribution(int idx,
                            inout vec3 ambient,
                            inout vec3 diffuse,
                            inout vec3 specular);

void main() {
  vec3 ambient = vec3(0);
  vec3 diffuse = vec3(0);
  vec3 specular = vec3(0);
  for(int i = 0; i < _num_lights; i++) {
    add_light_contribution(i, ambient, diffuse, specular);
  }

  vec3 light_color = diffuse + specular + ambient;

  _frag_color_out = vec4(light_color * _material.color, _material.alpha);
}

void add_light_contribution(int idx,
                            inout vec3 ambient,
                            inout vec3 diffuse,
                            inout vec3 specular) {
  float distance = length(_light.position - _frag_pos);
  float attenuation = 1.0 / (_light.attenuation.x +
                             _light.attenuation.y * distance +
                             _light.attenuation.z * distance * distance);

  ambient += attenuation *_light.strength.x * _light.ambient;

  vec3 l = normalize(_light.position - _frag_pos);
  float cos_t = clamp(dot(_frag_normal, l), 0, 1);
  diffuse = attenuation * _light.strength.y * cos_t * _light.color;

  vec3 c = normalize(_cam_pos - _frag_pos);
  float l_reflected_on_c = dot(normalize(reflect(-l, _frag_normal)), c);
  float spec = pow( max(l_reflected_on_c, 0.0), _material.shininess );
  specular += attenuation * _light.strength.z * spec * _light.color;
}
