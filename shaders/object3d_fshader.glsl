#version 330 core

const int POINTLIGHT = 0;
const int DIRECTLIGHT = 1;

in vec3 _frag_pos;
in vec3 _frag_normal;

out vec4 _frag_color_out;

uniform vec3 _cam_pos;
uniform struct Light {
  int type;
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 color;
  vec3 attenuation;
} _lights[5];
uniform struct Material {
  vec3 color;
  vec3 strength;
  float shininess;
  float alpha;
} _material;
uniform int _num_lights = 1;

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

// L = attenuate(k_d*L_d*(light.normal) + k_s*L_s*(reflected.view)^shine) + k_a*L_a
void add_light_contribution(int idx,
                            inout vec3 ambient,
                            inout vec3 diffuse,
                            inout vec3 specular) {
  float distance = length(_lights[idx].position - _frag_pos);
  float attenuation = (_lights[idx].type == DIRECTLIGHT) ? 1 : 1.0 / (_lights[idx].attenuation.x +
                                                                      _lights[idx].attenuation.z * distance * distance);

  ambient += _material.strength.x * _lights[idx].ambient;

  vec3 l = (_lights[idx].type == DIRECTLIGHT) ? normalize(-_lights[idx].direction) : normalize(_lights[idx].position - _frag_pos);
  float cos_t = clamp(dot(_frag_normal, l), 0, 1);
  diffuse = attenuation * _material.strength.y * cos_t * _lights[idx].color;

  vec3 c = normalize(_cam_pos - _frag_pos);
  float l_reflected_on_c = dot(normalize(reflect(-l, _frag_normal)), c);
  float spec = pow( max(l_reflected_on_c, 0.0), _material.shininess );
  specular += attenuation * _material.strength.z * spec * _lights[idx].color;
}
