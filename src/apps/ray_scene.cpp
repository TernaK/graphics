#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <graphics/ray.h>
#include <opencv2/opencv.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  
//  auto tree = Octree<int>(2);
//  cout << tree.get_level();
  
  auto mesh = make_shared<Primitive>(PrimitiveType::plane);
  
  ray_t ray;
  ray.d = glm::normalize(glm::vec3(0,-0.5,0.5));
  ray.p = glm::vec3(0,0.5,0);
  
  hit_t hit;
  Transformable transformable;
//  transformable.rotation.x = 45;
  transform_t transform = transformable.get_transform();
  if(mesh->ray_hit_test(ray, hit, transform))
    cout << "hit p: " << glm::to_string(hit.p) << " n: " << glm::to_string(hit.n) << endl;
}
