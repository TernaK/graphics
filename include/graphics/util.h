#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>

namespace graphics {
  namespace util {
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

    void glfw_error_callback(int err, const char* desc) {
      fprintf(stderr, "error [%d]: %s\n", err, desc);
      glfwTerminate();
    }
    
    void check_gl_errors() {
      GLenum err;
      while((err = glGetError()) && err != GL_NO_ERROR) {
        throw std::runtime_error(std::string("GL_ERROR(" + std::to_string(err) + ")\n"));
      }
    }
  }
}
