#include <graphics/scene.h>
#include <graphics/solid.h>
#include <graphics/sprite.h>
using namespace std;

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);
  
  auto texture = make_shared<graphics::Texture>("/Users/Terna/Downloads/1387868_720.jpg");
  auto sprite = make_shared<graphics::Sprite>(texture);
  
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
  std::shared_ptr<graphics::Solid>
  solid = std::make_shared<graphics::Solid>(vertices, indices, graphics::Material());
  
  scene.set_background(sprite);
  scene.add_drawable(solid);

  while(!glfwWindowShouldClose(canvas->window)) {
    solid->rotation.y += 1;
    scene.draw_scene();
  }
}
