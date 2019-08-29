#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <exception>
#include <unordered_map>
#include <map>
#include <array>

//--------------------------------------------------Window - interface
/// manage opengl context/window
class Window {
public:
  glm::vec3 clear_color { 0.2, 0.2, 0.2 };

  Window(glm::ivec2 size);
  ~Window();
  GLFWwindow* operator()() { return window; }
  void swap_buffers() { glfwSwapBuffers(window); }
  bool should_close() { return glfwWindowShouldClose(window); }
  void close() { glfwSetWindowShouldClose(window, true); }
  void destroy() { glfwDestroyWindow(window); }
  glm::ivec2 get_size() { return size; }
  glm::ivec2 get_framebuffer_size();
  void clear(bool color = true, bool depth = true);
  void set_depth_testing(bool val) { val ? glEnable(GL_DEPTH_TEST) :  glDisable(GL_DEPTH_TEST); };

  static void poll() { glfwPollEvents(); };
  static void check_errors();

private:
  GLFWwindow* window;
  glm::ivec2 size;
};

//--------------------------------------------------Shader - interface
/// interface to shaders, their uniforms & attributes
class Shader {
public:
  Shader(std::string v_text, std::string f_text);
  ~Shader();
  void use();
  GLint add_uniform(std::string name);
  GLint get_uniform(std::string name);
  GLint add_attribute(std::string name);
  GLint get_attribute(std::string name);
  void set_uniform(std::string uniform, float value);
  void set_uniform(std::string uniform, int value);
  void set_uniform(std::string uniform, bool value);
  void set_uniform(std::string uniform, const glm::vec2& value);
  void set_uniform(std::string uniform, const glm::vec3& value);
  void set_uniform(std::string uniform, const glm::vec4& value);
  void set_uniform(std::string uniform, const glm::mat2& value);
  void set_uniform(std::string uniform, const glm::mat3& value);
  void set_uniform(std::string uniform, const glm::mat4& value);

private:
  GLuint shader_program;
  std::unordered_map<std::string, GLint> uniforms;
  std::unordered_map<std::string, GLint> attributes;
};

//--------------------------------------------------BufferObject
/// hold device data
template<GLenum target, typename T = GLfloat>
struct BufferObject {
  GLuint buffer_object = 0;

  BufferObject() = default;

  BufferObject(T* data_ptr, int length, GLenum usage = GL_STATIC_DRAW) {
    glGenBuffers(1, &buffer_object);
    glBindBuffer(target, buffer_object);
    glBufferData(target, length * sizeof(T), data_ptr, usage);
    glBindBuffer(target, 0);
  }

  ~BufferObject() = default;

  void bind() { glBindBuffer(target, buffer_object); }

  void unbind() { glBindBuffer(target, 0); }

  void release() {
    if(buffer_object == 0) return;
    glDeleteBuffers(1, &buffer_object);
    buffer_object = 0;
  }
};

//--------------------------------------------------Texture
struct Texture {
  GLuint texture;
  int texture_unit { 0 };

  Texture() = default;

  /// create a texture from raw data
  Texture(unsigned char* data, int width, int height, bool bgr = true);

  /// activate the associated texture unit and bind to it
  void activate();
};

Texture::Texture(unsigned char* data, int width, int height, bool bgr) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  auto pixel_format = bgr ? GL_BGR : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
               pixel_format, GL_UNSIGNED_BYTE, (void*)data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate() {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}

//--------------------------------------------------main
int main() {
  Window window({640,480});
  std::cerr << "window size: " << glm::to_string(window.get_size()) << std::endl;
  std::cerr << "framebuffer size: " << glm::to_string(window.get_framebuffer_size()) << std::endl;

  //vertex shader
  std::string v_shader =
  R"(
  #version 330 core
  layout (location = 0) in vec3 pos;
  layout (location  = 1) in vec2 tex;
  out vec2 frag_tex_coord;
  uniform mat4 model;
  uniform vec3 offsets[4];

  void main() {
    vec3 offset = offsets[gl_InstanceID];
    gl_Position = vec4(offset,0) + model * vec4(pos,1);
    frag_tex_coord = tex;
  }
  )";

  //fragment shader
  std::string f_shader =
  R"(
  #version 330 core
  in vec2 frag_tex_coord;
  out vec4 color;
  uniform sampler2D texture0;

  void main() {
    color = texture(texture0, frag_tex_coord);
  }
  )";

  //shader
  Shader shader(v_shader, f_shader);
  shader.use();
  shader.add_attribute("pos");
  shader.add_attribute("tex");
  shader.add_uniform("model");
  shader.add_uniform("texture0");
  
  //data
  auto font = cv::FONT_HERSHEY_SIMPLEX;
  cv::Mat image0 = cv::Mat::ones(640,480,CV_8UC3) * 100;
  cv::putText(image0, "opengl + opencv", {30,image0.rows/2}, font, 1.5, {100,255,50}, 2);
  Texture texture0(image0.data, image0.cols, image0.rows);

  std::vector<GLfloat> pos_data {
    -0.2,-0.2,0, 0.2,-0.2,0, 0.2,0.2,0,
    0.2,0.2,0, -0.2,0.2,0, -0.2,-0.2,0
  };
  std::vector<GLfloat> tex_data {
    0,1, 1,1, 1,0,
    1,0, 0,0, 0,1,
  };
  auto pos_buffer = BufferObject<GL_ARRAY_BUFFER>(pos_data.data(), sizeof(pos_data));
  auto color_buffer = BufferObject<GL_ARRAY_BUFFER>(tex_data.data(), sizeof(tex_data));
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  pos_buffer.bind();
  glEnableVertexAttribArray(shader.get_attribute("pos"));
  glVertexAttribPointer(shader.get_attribute("pos"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
  pos_buffer.unbind();
  color_buffer.bind();
  glEnableVertexAttribArray(shader.get_attribute("tex"));
  glVertexAttribPointer(shader.get_attribute("tex"), 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)0);
  color_buffer.unbind();
  glBindVertexArray(0);

  std::vector<glm::vec3> offsets = {
    glm::vec3(-0.7,0.7,0),
    glm::vec3(0.7,0.7,0),
    glm::vec3(-0.7,-0.7,0),
    glm::vec3(0.7,-0.7,0)
  };
  for(size_t i = 0; i < 4; i++) {
    std::string uniform = cv::format("offsets[%d]", i);
    shader.add_uniform(uniform);
    shader.set_uniform(uniform, offsets[i]);
  }

  //render
  int i = 0;
  while(!window.should_close()) {
    i++;
    window.clear();

    shader.use();

    float angle = sin(2*M_PI*0.05*glfwGetTime()) * 2 * M_PI;
    glm::mat4 scale = glm::scale(glm::vec3(1.0));
    glm::mat4 model = glm::rotate(scale, angle, glm::vec3(0,0,1));
    shader.set_uniform("model", model);

    texture0.texture_unit = 0;
    texture0.activate();
    shader.set_uniform("texture0", texture0.texture_unit);

    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 9, 4);
    glBindVertexArray(0);

    window.check_errors();
    window.swap_buffers();
    Window::poll();
  }
}

//--------------------------------------------------Window - implementation
Window::Window(glm::ivec2 size) : size(size) {
  glewExperimental = true;
  glfwSetErrorCallback([](int code, const char* message) {
    std::cerr << "glfw error: " << message << std::endl;
    glfwTerminate();
  });
  //init before hints
  if(!glfwInit())
    throw std::runtime_error("could not initialize glfw");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, false);
  //create window/context
  window = glfwCreateWindow(640, 480, "gl_test", nullptr, nullptr);
  if(!window) {
    glfwTerminate();
    throw std::runtime_error("could not intialize glew");
  }
  glfwMakeContextCurrent(window);
  //extensions need context
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    std::cerr << glewGetErrorString(errno) << std::endl;
    throw std::runtime_error("could not intialize glew");
  }
  //close callback
  glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {
    glfwSetWindowShouldClose(win, true);
  });
  //enable depth testing
  set_depth_testing(true);
}

Window::~Window() {
  close();
  destroy();
  glfwTerminate();
}

glm::ivec2 Window::get_framebuffer_size() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return glm::ivec2{width,height};
}

void Window::clear(bool color, bool depth) {
  if(color || depth)
    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}

void Window::check_errors() {
  GLenum err;
  while((err = glGetError()) && err != GL_NO_ERROR) {
    switch(err) {
      case GL_INVALID_ENUM:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_ENUM)\n"));
        break;
      case GL_INVALID_OPERATION:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_OPERATION)\n"));
        break;
      case GL_INVALID_VALUE:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_VALUE)\n"));
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        throw std::runtime_error(std::string("GL_ERROR(GL_INVALID_FRAMEBUFFER_OPERATION)\n"));
        break;
      default:
        throw std::runtime_error(std::string("GL_ERROR(" + std::to_string(err) + ")\n"));
    }
  }
}

//--------------------------------------------------Shader - implementation
Shader::Shader(std::string v_text, std::string f_text) {
  int success;
  char infoLog[512];
  //vertex shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char* vshader_ptr = v_text.c_str();
  glShaderSource(vertex_shader, 1, &vshader_ptr, nullptr);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
    std::cerr << "vertex shader error: " << infoLog << std::endl;
  }
  //fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fshader_ptr = f_text.c_str();
  glShaderSource(fragment_shader, 1, &fshader_ptr, nullptr);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
    std::cerr << "fragment shader error: " << infoLog << std::endl;
  }
  //program
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);
  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
    std::cerr << "shader program error: " << infoLog << std::endl;
  }
  //cleanup
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

Shader::~Shader() {
  glDeleteProgram(shader_program);
}

void Shader::use() {
  glUseProgram(shader_program);
}

GLint Shader::add_uniform(std::string uniform_name) {
  auto iter = uniforms.find(uniform_name);
  if(iter != uniforms.end())
    return iter->second;
  //if not already found get
  GLint location = glGetUniformLocation(shader_program, uniform_name.c_str());
  if(location == -1)
    throw std::runtime_error(uniform_name + " uniform not found in shader");
  uniforms.insert({uniform_name, location});
  return location;
}

GLint Shader::add_attribute(std::string attribute_name) {
  auto iter = attributes.find(attribute_name);
  if(iter != attributes.end())
    return iter->second;
  //if not already found get
  GLint location = glGetAttribLocation(shader_program, attribute_name.c_str());
  if(location == -1)
    throw std::runtime_error(attribute_name + " attribute not found in shader");
  attributes.insert({attribute_name, location});
  return location;
}

GLint Shader::get_uniform(std::string uniform_name) {
  if(uniforms.find(uniform_name) == uniforms.end())
    throw std::runtime_error(uniform_name + " uniform not found in shader");
  return uniforms[uniform_name];
}

GLint Shader::get_attribute(std::string attribute_name) {
  if(attributes.find(attribute_name) == attributes.end())
    throw std::runtime_error(attribute_name + " attribute not found in shader");
  return attributes[attribute_name];
}

void Shader::set_uniform(std::string uniform, float value) {
  glUniform1f(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, int value) {
  glUniform1i(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, bool value) {
  glUniform1i(uniforms.at(uniform), value);
}

void Shader::set_uniform(std::string uniform, const glm::vec2& value) {
  glUniform2fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec3& value) {
  glUniform3fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::vec4& value) {
  glUniform4fv(uniforms.at(uniform), 1, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat2& value) {
  glUniformMatrix2fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat3& value) {
  glUniformMatrix3fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set_uniform(std::string uniform, const glm::mat4& value) {
  glUniformMatrix4fv(uniforms.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}
