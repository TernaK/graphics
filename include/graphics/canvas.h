#pragma once
#include <graphics/shader.h>

namespace graphics {
  struct Helper {
    static GLFWwindow* make_window(int w, int h, std::string name) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
    Canvas(int width = 800, int height = 600, std::string name = "canvas");
    ~Canvas();
    GLFWwindow* window;
  };
}
