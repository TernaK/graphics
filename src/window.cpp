#include "graphics/window.h"
using namespace graphics;

//--------------------------------------------------Window - implementation
Window::Window(cv::Size size) : size(size) {
  glewExperimental = true;
  glfwSetErrorCallback([](int code, const char* message) {
    std::cerr << "glfw error: " << message << std::endl;
    glfwTerminate();
  });
  //init before hints
  if(!glfwInit())
    throw std::runtime_error("could not initialize glfw");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, false);
  //create window/context
  window = glfwCreateWindow(640, 480, "gl_test", nullptr, nullptr);
  if(!window) {
    glfwTerminate();
    throw std::runtime_error("could not intialize glew");
  }
  glfwMakeContextCurrent(window);
  //extensions need context
  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    std::cerr << glewGetErrorString(errno) << std::endl;
    throw std::runtime_error("could not intialize glew");
  }
  //close callback
  glfwSetWindowCloseCallback(window, [](GLFWwindow* win) {
    glfwSetWindowShouldClose(win, true);
  });
  //enable depth testing
  set_depth_testing(true);
}

Window::~Window() {
  close();
  destroy();
  glfwTerminate();
}

cv::Size Window::get_framebuffer_size() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return {width,height};
}

void Window::clear(bool color, bool depth) {
  if(color || depth)
    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}
