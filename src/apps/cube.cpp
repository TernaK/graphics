#include <graphics/node.h>
#include <graphics/shader.h>
#include <graphics/util.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace std;

int main(int argc, char* args[]) {
  if(!glfwInit()) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetErrorCallback(graphics::util::glfw_error_callback);

  int win_width = 800;
  int win_height = 600;
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
  std::vector<glm::vec4> colors(8);
  std::fill(colors.begin(), colors.begin()+4, glm::vec4(1,0,0,1));
  std::fill(colors.begin()+4, colors.end(), glm::vec4(0,0,1,1));
  std::vector<GLint> indices = {
    0,1,2, 1,3,2, //front
    4,5,6, 5,7,6,//back
    5,0,7, 0,2,7,//left
    1,4,3, 4,6,3,//right
    5,4,0, 4,1,0,//top
    2,3,7, 3,6,7//bottom
  };
  graphics::Node cube(vertices, colors, indices);
  cube.scale = glm::vec3(0.7);
  
  graphics::Shader shader(string(GRAPHICS_SHADERS_DIRECTORY) + "3d_vshader.glsl",
                          string(GRAPHICS_SHADERS_DIRECTORY) + "3d_fshader.glsl");
  
  shader.use();
  glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), GLfloat(win_width)/GLfloat(win_height), 0.1f, 50.f);
  GLint loc = glGetUniformLocation(shader.shader_program, "_proj_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj_mat));

  glm::mat4 view_mat = glm::lookAt(glm::vec3(0,5,8), glm::vec3(0,0,0), glm::vec3(0,1,0));
  loc = glGetUniformLocation(shader.shader_program, "_view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view_mat));

  while(!glfwWindowShouldClose(window)) {
    glClearColor(0.1, 0.6, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    cube.translation.x = 2 * sin(2*M_PI*2*glfwGetTime() / 5.0);
    cube.translation.z = 2 * cos(2*M_PI*2*glfwGetTime() / 5.0);
    cube.rotation.x += 2;
    cube.rotation.y += 5;

    cube.render(shader);
    glfwPollEvents();
    glfwSwapBuffers(window);
    graphics::util::check_gl_errors();
  }
  /* code */

  glfwDestroyWindow(window);
  glfwTerminate();
}
