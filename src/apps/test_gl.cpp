#define GLM_ENABLE_EXPERIMENTAL
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/buffer_object.h"
#include "graphics/texture.h"
#include <vector>

//--------------------------------------------------main
int main() {
  //glfwWindowHint(GL_SAMPLES, 4);
  graphics::Window window({640,480});
  //glEnable(GL_MULTISAMPLE);
  std::cerr << "window size: " << window.get_size() << std::endl;
  std::cerr << "framebuffer size: " << window.get_framebuffer_size() << std::endl;

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
  graphics::Shader shader(v_shader, f_shader);
  shader.use();
  shader.add_attribute("pos");
  shader.add_attribute("tex");
  shader.add_uniform("model");
  shader.add_uniform("texture0");

  //data
  auto font = cv::FONT_HERSHEY_SIMPLEX;
  cv::Mat image0 = cv::Mat::ones(480,480,CV_8UC3) * 100;
  cv::putText(image0, "opengl + opencv", {30,image0.rows/2}, font, 1.5, {100,255,50}, 2);
  graphics::Texture texture0(image0.data, image0.cols, image0.rows);

  std::vector<GLfloat> pos_data {
    -0.25,-0.25,0, 0.25,-0.25,0, 0.25,0.25,0,
    0.25,0.25,0, -0.25,0.25,0, -0.25,-0.25,0
  };
  std::vector<GLfloat> tex_data {
    0,1, 1,1, 1,0,
    1,0, 0,0, 0,1,
  };
  auto pos_buffer = graphics::BufferObject<GL_ARRAY_BUFFER>(pos_data.data(), sizeof(pos_data));
  auto color_buffer = graphics::BufferObject<GL_ARRAY_BUFFER>(tex_data.data(), sizeof(tex_data));
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
    glm::vec3(-0.5,0.5,0),
    glm::vec3(0.5,0.5,0),
    glm::vec3(-0.5,-0.5,0),
    glm::vec3(0.5,-0.5,0)
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

    graphics::check_errors();
    window.swap_buffers();
    graphics::Window::poll();
  }
}
