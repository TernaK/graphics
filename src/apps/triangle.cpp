#include <graphics/canvas.h>
#include <graphics/shader.h>
#include <memory>
#include <vector>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  auto canvas = make_shared<Canvas>(800,600);
  Shader shader = Shader::make_simple2d_shader();

  std::vector<float> data = {
    0,0.5,0, 1,0,0,1,
    -0.5,-0.5,0, 0,1,0,1,
    0.5,-0.5,0, 0,0,1 ,1,
  };

  shader.add_attribute("_color");
  shader.add_attribute("_pos");

  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(shader["_pos"], 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(shader["_color"], 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 1.0);

  shader.use();
  while(canvas->still_open()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    canvas->swap_buffers();
  }
  void close_window();
}

