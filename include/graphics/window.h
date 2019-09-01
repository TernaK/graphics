#pragma once
#include "graphics.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <exception>

namespace graphics {
  //--------------------------------------------------Window - interface
  /// manage opengl context/window
  class Window {
  public:
    glm::vec3 clear_color { 0.2, 0.2, 0.2 };

    Window(cv::Size size);
    ~Window();
    GLFWwindow* operator()() { return window; }
    void swap_buffers() { glfwSwapBuffers(window); }
    bool should_close() { return glfwWindowShouldClose(window); }
    void close() { glfwSetWindowShouldClose(window, true); }
    void destroy() { glfwDestroyWindow(window); }
    cv::Size get_size() { return size; }
    cv::Size get_framebuffer_size();
    void clear(bool color = true, bool depth = true);
    void set_depth_testing(bool val) { val ? glEnable(GL_DEPTH_TEST) :  glDisable(GL_DEPTH_TEST); };

    static void poll() { glfwPollEvents(); };
    static void check_errors();

  private:
    GLFWwindow* window;
    cv::Size size;
  };
}
