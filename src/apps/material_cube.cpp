#include <graphics/util.h>
#include <graphics/renderer.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;

int main(int argc, char* args[]) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(graphics::util::glfw_error_callback);

  int win_width = 720;
  int win_height = 360;
  GLFWwindow* window = graphics::util::make_window(win_width, win_height, "cube");
  glfwMakeContextCurrent(window);

  if(!(glewInit() == GLEW_OK)) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, graphics::util::resize_callback);
  glfwSetKeyCallback(window, graphics::util::esc_close_callback);

  /* code */
  glEnable(GL_DEPTH_TEST);
  std::vector<glm::vec3> vertices = {
    glm::vec3(-1,1,1), glm::vec3(1,1,1),      //front top left 0, top right 1
    glm::vec3(-1,-1,1), glm::vec3(1,-1,1),    //front low left 2, low right 3
    glm::vec3(1,1,-1), glm::vec3(-1,1,-1),    //back top left 4, top right 5
    glm::vec3(1,-1,-1), glm::vec3(-1,-1,-1),  //back low left 6, low right 7
  };
  std::vector<GLint> indices = {
    0,1,2, 1,3,2, //front
    4,5,6, 5,7,6,//back
    5,0,7, 0,2,7,//left
    1,4,3, 4,6,3,//right
    5,4,0, 4,1,0,//top
    2,3,7, 3,6,7//bottom
  };
  graphics::MaterialNode cube(vertices, indices, graphics::Material());
  
  shared_ptr<graphics::Shader>
  shader = make_shared<graphics::Shader>(string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_vshader.glsl",
                                         string(GRAPHICS_SHADERS_DIRECTORY) + "3d_material_point_fshader.glsl");
  
  shared_ptr<graphics::Camera> camera = make_shared<graphics::Camera>();
  camera->aspect_ratio = GLfloat(win_width)/win_height;
  camera->position = glm::vec3(0,5,8);

  shared_ptr<graphics::PointLight> light = make_shared<graphics::PointLight>();
  
  graphics::Renderer renderer = graphics::Renderer(shader, camera, light);

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cube.scale = glm::vec3(0.7);
    cube.position.x = 2 * sin(2*M_PI*glfwGetTime() * 0.1);
    cube.position.z = 2 * cos(2*M_PI*glfwGetTime() * 0.1);
    cube.rotation.x += 2;
    cube.rotation.y += 1;
    cube.material.color = glm::vec3(0.6, 0.2, 0.7);
    
    renderer.render(cube);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    cube.scale = glm::vec3(1.5);
    cube.position.x = 2 * cos(2*M_PI*glfwGetTime() * 0.1);
    cube.position.z = 2 * sin(2*M_PI*glfwGetTime() * 0.1);
    cube.rotation.x += 2;
    cube.rotation.y += 1;
    cube.material.color = glm::vec3(0.1, 0.7, 0.6);
    
    renderer.render(cube);
    
    glfwPollEvents();
    glfwSwapBuffers(window);
    graphics::util::check_gl_errors();
  }
  /* code */

  glfwDestroyWindow(window);
  glfwTerminate();
}
