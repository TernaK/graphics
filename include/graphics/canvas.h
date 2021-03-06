#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>

namespace graphics {
  struct CanvasHelper {
    static GLFWwindow* make_window(int w, int h, bool hidden, std::string name) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      if(hidden)
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
      GLFWwindow* window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
      if(!window)
        throw std::runtime_error("window could not be created");
      return window;
    }

    static void glfw_error_callback(int err, const char* desc) {
      fprintf(stderr, "error [%d]: %s\n", err, desc);
      glfwTerminate();
    }

    static void check_gl_errors() {
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

    static void resize_callback(GLFWwindow* window, int w, int h) {
      glViewport(0, 0, w, h);
    }

    static void esc_close_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      if(action == GLFW_RELEASE) {
        switch(key) {
          case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
      }
    }
  };

  class Canvas {
  public:
    int width;
    int height;
    std::string name;
    GLFWwindow* window;
    
    Canvas(int width = 800, int height = 600, bool hidden = false, std::string name = "canvas");
    ~Canvas();
    bool still_open();
    void close_window();
    void get_true_frame_size(int& width, int& height);
    void swap_buffers();
    float get_aspect_ratio();
    void clear(bool color = true, bool depth = true);
    void poll_events();
  };
}
