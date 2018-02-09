#include <graphics/node.h>
#include <graphics/util.h>
#include <graphics/shader.h>

using namespace std;

namespace helper {
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE) {
      switch(key) {
        case GLFW_KEY_ESCAPE:
          glfwSetWindowShouldClose(window, GLFW_TRUE);
          break;
      }
    }
  }
}

int main(int argc, char* args[]) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(graphics::util::glfw_error_callback);
  
  GLFWwindow* window = graphics::util::make_window(800, 600, "triangle");
  glfwMakeContextCurrent(window);
  
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetFramebufferSizeCallback(window, graphics::util::resize_callback);
  glfwSetKeyCallback(window, helper::key_callback);
  
  /* code */
  std::vector<glm::vec3> vertices = {
    glm::vec3(0,0.5,0),
    glm::vec3(-0.5,-0.5,0),
    glm::vec3(0.5,-0.5,0)
  };
  std::vector<glm::vec4> colors = {
    glm::vec4(1,0,0,1),
    glm::vec4(0,1,0,1),
    glm::vec4(0,0,1,1)
  };
  std::vector<GLint> indices = { 0, 1, 2 };
  graphics::Node node(vertices, colors, indices);
  graphics::Shader shader(string(GRAPHICS_SHADERS_DIRECTORY) + "2d_vshader.glsl",
                          string(GRAPHICS_SHADERS_DIRECTORY) + "2d_fshader.glsl");
  shader.use();

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.1, 0.4, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    node.render();
    glfwPollEvents();
    glfwSwapBuffers(window);
    graphics::util::check_gl_errors();
  }
  /* code */
  
  glfwDestroyWindow(window);
  glfwTerminate();
}
