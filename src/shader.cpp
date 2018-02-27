#include <graphics/shader.h>

using namespace std;
using namespace graphics;

Shader::Shader() {

}

Shader::~Shader() {
  glDeleteProgram(shader_program);
}

Shader::Shader(std::string vshader_path, std::string fshader_path) {
  ifstream file;
  file.open(vshader_path);
  if(!file.is_open())
    throw std::runtime_error("vertex shader not found: " + vshader_path);

  stringstream v_ss;
  v_ss << file.rdbuf();
  string v_string = v_ss.str();
  file.close();

  file.open(fshader_path);
  if(!file.is_open())
    throw std::runtime_error("fragment shader not found: " + fshader_path);

  stringstream f_ss;
  f_ss << file.rdbuf();
  string f_string = f_ss.str();
  file.close();

  create_shader_program(v_string, f_string);
}

void Shader::create_shader_program(std::string vshader_str, std::string fshader_str) {
  int success;
  char infoLog[512];

  //vertex shader
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vshader_ptr = vshader_str.c_str();
  glShaderSource(vertex_shader, 1, &vshader_ptr, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    cerr << "vertex shader error: " << infoLog << endl;
  }

  //fragment shader
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fshader_ptr = fshader_str.c_str();
  glShaderSource(fragment_shader, 1, &fshader_ptr, nullptr);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    cerr << "fragment shader error: " << infoLog << endl;
  }

  //program
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    cerr << "shader program error: " << infoLog << endl;
  }

  //cleanup
//  glDetachShader(shader_program, vertex_shader);
//  glDetachShader(shader_program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::use() const {
  glUseProgram(shader_program);
}

GLint Shader::add_uniform(std::string uniform_name) {
  auto iter = uniforms.find(uniform_name);
  if(iter != uniforms.end()) {
    return iter->second;
  }

  GLint location = glGetUniformLocation(shader_program, uniform_name.c_str());
  if(location == -1)
    throw runtime_error(uniform_name + " uniform not found in shader");
  uniforms[uniform_name] = location;
  return location;
}

GLint Shader::add_attribute(std::string attribute_name) {
  auto iter = attributes.find(attribute_name);
  if(iter != attributes.end()) {
    return iter->second;
  }

  GLint location = glGetAttribLocation(shader_program, attribute_name.c_str());
  if(location == -1)
    throw runtime_error(attribute_name + " attribute not found in shader");
  attributes[attribute_name] = location;
  return location;
}

GLint Shader::operator()(std::string uniform_name) {
  if(uniforms.find(uniform_name) == uniforms.end())
    throw runtime_error(uniform_name + " uniform not found in shader");
  return uniforms[uniform_name];
}

GLint Shader::operator[](std::string attribute_name) {
  if(attributes.find(attribute_name) == attributes.end())
    throw runtime_error(attribute_name + " attribute not found in shader");
  return attributes[attribute_name];
}

std::shared_ptr<Shader> Shader::make_simple2d_shader() {
  auto shader = make_shared<Shader>(graphics::SHADERS_DIR + "simple2d_vshader.glsl",
                                    graphics::SHADERS_DIR + "simple2d_fshader.glsl");
  shader->name = "simple2d";
  shader->use();
  shader->add_attribute("_pos");
  shader->add_attribute("_color");
  return shader;
}

std::shared_ptr<Shader> Shader::make_mesh_point_shader() {
  auto shader = make_shared<Shader>(graphics::SHADERS_DIR + "mesh_point_vshader.glsl",
                                    graphics::SHADERS_DIR + "mesh_point_fshader.glsl");
  shader->name = "mesh_point";
  shader->use();
  shader->add_attribute("_pos");
  shader->add_attribute("_color");
  shader->add_attribute("_normal");
  shader->add_uniform("_light.color");
  shader->add_uniform("_light.ambient");
  shader->add_uniform("_light.position");
  shader->add_uniform("_light.attenuation");
  shader->add_uniform("_proj_mat");
  shader->add_uniform("_view_mat");
  shader->add_uniform("_model_mat");
  shader->add_uniform("_normal_mat");
  shader->add_uniform("_cam_pos");
  return shader;
}

std::shared_ptr<Shader> Shader::make_object3d_shader() {
  auto shader = make_shared<Shader>(graphics::SHADERS_DIR + "object3d_vshader.glsl",
                                    graphics::SHADERS_DIR + "object3d_fshader.glsl");
  shader->name = "object3d";
  shader->use();
  shader->add_attribute("_pos");
  shader->add_attribute("_normal");
  shader->add_uniform("_material.color");
  shader->add_uniform("_material.alpha");
  shader->add_uniform("_material.shininess");
  shader->add_uniform("_material.strength");
  shader->add_uniform("_num_lights");
  shader->add_uniform("_proj_mat");
  shader->add_uniform("_view_mat");
  shader->add_uniform("_model_mat");
  shader->add_uniform("_normal_mat");
  shader->add_uniform("_cam_pos");
  return shader;
}

std::shared_ptr<Shader> Shader::make_sprite_shader() {
  auto shader = make_shared<Shader>(graphics::SHADERS_DIR + "sprite_vshader.glsl",
                                    graphics::SHADERS_DIR + "sprite_fshader.glsl");
  shader->name = "sprite";
  shader->use();
  shader->add_attribute("_pos");
  shader->add_attribute("_tex_coord");
  shader->add_uniform("_texture0");
  return shader;
}

void Shader::set_uniform(std::string uniform, float value) {
  glUniform1f((*this)(uniform), value);
}

void Shader::set_uniform(std::string uniform, int value) {
  glUniform1i((*this)(uniform), value);
}

void Shader::set_uniform(std::string uniform, bool value) {
  glUniform1i((*this)(uniform), value);
}

void Shader::set_uniform(std::string uniform, const glm::vec2& value) {
  glUniform2fv((*this)(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec3& value) {
  glUniform3fv((*this)(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec4& value) {
  glUniform4fv((*this)(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat2& value) {
  glUniformMatrix2fv((*this)(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat3& value) {
  glUniformMatrix3fv((*this)(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat4& value) {
  glUniformMatrix4fv((*this)(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

