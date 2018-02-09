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
