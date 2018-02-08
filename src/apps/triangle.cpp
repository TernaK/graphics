#include <graphics/node.h>
#include <graphics/util.h>
#include <graphics/shader.h>

using namespace std;

namespace helper {
  void resize_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
  }
  
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
  
  glfwSetErrorCallback(graphics::util::error_callback);
  
  GLFWwindow* window = graphics::util::make_window(800, 600, "triangle");
  glfwMakeContextCurrent(window);
  
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetFramebufferSizeCallback(window, helper::resize_callback);
  glfwSetKeyCallback(window, helper::key_callback);
  
  /* code */
//  std::vector<glm::vec3> vertices = {
//    glm::vec3(0.5,0.5,0),
//    glm::vec3(-0.5,-0.5,0),
//    glm::vec3(0.5,-0.5,0)
//  };
//  graphics::Node node(vertices, {});
  graphics::Shader shader(string(GRAPHICS_SHADERS_DIRECTORY) + "vshader.glsl",
                          string(GRAPHICS_SHADERS_DIRECTORY) + "fshader.glsl");


  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.1, 0.4, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  /* code */
  
  glfwDestroyWindow(window);
  glfwTerminate();
}
