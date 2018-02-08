#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

using namespace std;

namespace helper {
  void error_callback(int err, const char* desc) {
    fprintf(stderr, "error [%d]: %s\n", err, desc);
    glfwTerminate();
  }
  
  GLFWwindow* make_window(int w, int h, std::string name) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
    if(!window)
      throw std::runtime_error("window could not be created");
    return window;
  }
}

int main(int argc, char* args[]) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetErrorCallback(helper::error_callback);
  
  //create context before initializing glew
  GLFWwindow* window = helper::make_window(800, 600, "hello");
  glfwMakeContextCurrent(window);
  
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  /* code */
  while(!glfwWindowShouldClose(window)) {
    
    glfwPollEvents();
  }
  /* code */

  //cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
