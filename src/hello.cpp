#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void error_callback(int error, const char* desc) {
  fprintf(stderr, "error[%d]: %s\n", error, desc);
  glfwTerminate();
}

GLFWwindow* make_window(int w, int h, std::string name) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
  GLFWwindow* window = glfwCreateWindow(w, h, name.c_str(), NULL, NULL);
  if(!window)
    exit(EXIT_FAILURE);
  
  return window;
}

int main(int argc, char* args[]) {
  if(!glfwInit())
    exit(EXIT_FAILURE);
  
  glfwSetErrorCallback(error_callback);
  
  //create context before initializing glew
  GLFWwindow* window = make_window(800, 600, "hello");
  glfwMakeContextCurrent(window);
  
  glewInit();
  
  /* code */
  while(!glfwWindowShouldClose(window)) {
    
    glfwPollEvents();
  }
  /* code */

  //cleanup
  glfwDestroyWindow(window);
  glfwTerminate();
}
