#include <graphics/canvas.h>
#include <graphics/mesh.h>
#include <graphics/camera.h>
#include <graphics/light.h>
#include <memory>
#include <vector>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  auto canvas = make_shared<Canvas>(480,480);
  auto shader = make_shared<Shader>(Shader::make_mesh_shader());
  std::vector<glm::vec3> vertices = {
    glm::vec3(-1,1,1), glm::vec3(1,1,1),      //front top left 0, top right 1
    glm::vec3(-1,-1,1), glm::vec3(1,-1,1),    //front low left 2, low right 3
    glm::vec3(1,1,-1), glm::vec3(-1,1,-1),    //back top left 4, top right 5
    glm::vec3(1,-1,-1), glm::vec3(-1,-1,-1),  //back low left 6, low right 7
  };
  std::vector<GLint> indices = {
    0,1,2, 1,3,2, //front
    4,5,6, 5,7,6, //back
    5,0,7, 0,2,7, //left
    1,4,3, 4,6,3, //right
    5,4,0, 4,1,0, //top
    2,3,7, 3,6,7  //bottom
  };
  std::vector<glm::vec4> colors = {
    glm::vec4(1,0,0,1), glm::vec4(1,0,0,1), //front top left 0, top right 1
    glm::vec4(1,0,0,1), glm::vec4(1,0,0,1), //front low left 2, low right 3
    glm::vec4(0,1,0,1), glm::vec4(0,1,0,1), //back top left 4, top right 5
    glm::vec4(0,1,0,1), glm::vec4(0,1,0,1), //back low left 6, low right 7
  };
  Mesh mesh(vertices, colors, indices);
  PointLight light;
  Camera camera;
  camera.position.z = 10;
  camera.position.y = 5;

  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0, 1.0);

  shader->use();

  camera.set_uniforms(shader);
  light.set_uniforms(shader);
  while(canvas->still_open()) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh.draw(shader);
    mesh.rotation.y += 1;
    
    glfwPollEvents();
    canvas->swap_buffers();
  }
}
