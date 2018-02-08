#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

using namespace std;

namespace helper {
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
  
  void error_callback(int err, const char* desc) {
    fprintf(stderr, "error [%d]: %s\n", err, desc);
    glfwTerminate();
  }
  
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
  
  glfwSetErrorCallback(helper::error_callback);
  
  GLFWwindow* window = helper::make_window(800, 600, "triangle");
  glfwMakeContextCurrent(window);
  
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetFramebufferSizeCallback(window, helper::resize_callback);
  glfwSetKeyCallback(window, helper::key_callback);
  
  /* code */
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
