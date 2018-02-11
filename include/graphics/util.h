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

    void resize_callback(GLFWwindow* window, int w, int h) {
      glViewport(0, 0, w, h);
    }

    void esc_close_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      if(action == GLFW_RELEASE) {
        switch(key) {
          case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
      }
    }
  }
}