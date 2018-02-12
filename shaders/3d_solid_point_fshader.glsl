#version 330 core

in vec4 _frag_color;
in vec3 _frag_pos;
in vec3 _frag_normal;

out vec4 _frag_color_out;

uniform vec3 _cam_pos;
uniform struct Light {
  vec3 position;
  vec3 ambient;
  vec3 color;
  vec3 attenuation;
} _light;

void main() {
  vec3 l = normalize(_light.position - _frag_pos);
  float cos_t = clamp(dot(_frag_normal, l), 0, 1);
  vec3 diffuse = cos_t * _light.color;
  
  vec3 c = normalize(_cam_pos - _frag_pos);
  float l_reflected_on_c = dot(normalize(reflect(-l, _frag_normal)), c);
  float spec = pow( max(l_reflected_on_c, 0.0), 32 );
  vec3 specular = spec * _light.color;
  
  float distance = length(_light.position - _frag_pos);
  float attenuation = 1.0 / (_light.attenuation.x +
                             _light.attenuation.y * distance +
                             _light.attenuation.z * distance * distance);
  
  vec3 light_color = (diffuse + specular + _light.ambient) * attenuation;
  
  _frag_color_out = vec4(light_color * _frag_color.rgb, _frag_color.a);
}
