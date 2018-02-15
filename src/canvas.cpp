#include <graphics/canvas.h>

using namespace graphics;

Canvas::Canvas(int width, int height, bool hidden, std::string name)
: width(width), height(height), name(name) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(CanvasHelper::glfw_error_callback);

  window = CanvasHelper::make_window(width, height, hidden, name);
  glfwMakeContextCurrent(window);

  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, CanvasHelper::resize_callback);
  glfwSetKeyCallback(window, CanvasHelper::esc_close_callback);
  glEnable(GL_DEPTH_TEST);
}

Canvas::~Canvas() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool Canvas::still_open() {
  return glfwWindowShouldClose(window);
}

void Canvas::close_window() {
  return glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Canvas::get_true_frame_size(int& width, int& height) {
  glfwGetFramebufferSize(window, &width, &height);
}

void Canvas::swap_buffers() {
  glfwSwapBuffers(window);
}
