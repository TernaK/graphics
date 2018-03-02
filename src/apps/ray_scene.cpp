#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <graphics/implicit.h>
#include <graphics/ray.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  cv::Size frame_size(640, 480);
  int supersample_factor = 2;
  shared_ptr<Canvas> canvas = make_shared<Canvas>(frame_size.width,frame_size.height,true);

  auto sphere = std::make_shared<ImplicitNode>(ShapeType::sphere);
  sphere->scale = glm::vec3(2.0);
  sphere->position = glm::vec3(1,0,1);
  sphere->materials[0].color = glm::vec3(1.0,0.2,0.2);
  
  auto box = std::make_shared<ImplicitNode>(ShapeType::box);
  box->scale = glm::vec3(1.5);
  box->position = glm::vec3(-1.5,0,0);
  box->rotation = glm::vec3(10,0,0);
  box->materials[0].color = glm::vec3(0.2,0.9,0.2);
  
  auto plane = std::make_shared<ImplicitNode>(ShapeType::plane);
  plane->scale = glm::vec3(20);
  plane->materials[0].color = glm::vec3(0.2,0.2,0.85);
  plane->position.y = -3;

  //light
  std::shared_ptr<Light> light = make_shared<Light>();
  light->position = glm::vec3(3,5,5);

  //camera
  std::shared_ptr<Camera> camera = make_shared<Camera>();
  camera->position = glm::vec3(0,0,4);
  camera->aspect_ratio = canvas->get_aspect_ratio();

  Transformable parent_transformable;
  
  shared_ptr<RayScene> ray_scene = make_shared<RayScene>();
  ray_scene->add_node(sphere);
  ray_scene->add_node(box);
  ray_scene->add_node(plane);
  ray_scene->root->rotation.x = 10;
  
  auto scene_renderer = make_shared<RaySceneRenderer>(frame_size, camera);
  scene_renderer->supersample_factor = supersample_factor;
  scene_renderer->light = light;
  
  cv::Mat image = scene_renderer->render_scene(ray_scene);
  
  cv::imshow("ray cast", image);
  cv::waitKey();
}
