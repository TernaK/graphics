#include <graphics/canvas.h>

using namespace graphics;

Canvas::Canvas(int width, int height, std::string name)
: width(width), height(height), name(name) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(Helper::glfw_error_callback);

  window = Helper::make_window(width, height, name);
  glfwMakeContextCurrent(window);

  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, Helper::resize_callback);
  glfwSetKeyCallback(window, Helper::esc_close_callback);
  glEnable(GL_DEPTH_TEST);

  material_shader = std::make_shared<MaterialShader>();
  solid_shader = std::make_shared<SolidShader>();
}

Canvas::~Canvas() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
