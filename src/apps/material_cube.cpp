#include <graphics/scene.h>

using namespace std;

int main(int argc, char* args[]) {
  std::shared_ptr<graphics::Canvas> canvas = std::make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);

  /* code */
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

  std::shared_ptr<graphics::Solid>
  cube = std::make_shared<graphics::Solid>(vertices, indices, graphics::Material());

  std::shared_ptr<graphics::Node>
  node = std::make_shared<graphics::Node>(vertices, colors, indices);
  
  scene.add_drawable(cube);
  scene.add_drawable(node);

  while(!glfwWindowShouldClose(canvas->window)) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    cube->scale = glm::vec3(1.5);
    cube->position.x = 2 * cos(2*M_PI*glfwGetTime() * 0.1);
    cube->position.z = 2 * sin(2*M_PI*glfwGetTime() * 0.1);
    cube->rotation.x += 2;
    cube->rotation.y += 1;

    scene.draw_scene();
  }
  /* code */
}
