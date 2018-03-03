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

  std::vector<shared_ptr<ImplicitNode>> spheres;
  for(int i = -1; i <= 1; i++) {
    for(int j = -1; j <= 1; j++) {
      if(i == 0 && j == 0) continue;
      auto sphere = std::make_shared<ImplicitNode>(ShapeType::sphere);
      sphere->scale = glm::vec3(1);
      sphere->position = glm::vec3(j*2.5,i*2.5,1);
      sphere->materials[0].color = glm::vec3(fabs(j),fabs(i),0.2);
      spheres.push_back(std::move(sphere));
    }
  }

  auto box = std::make_shared<ImplicitNode>(ShapeType::box);
  box->materials[0].color = glm::vec3(1,1,0.2);
  box->rotation = glm::vec3(44,44,0);
  box->position.z = 1.2;
  box->scale = glm::vec3(0.5);

  auto plane = std::make_shared<ImplicitNode>(ShapeType::plane);
  plane->scale = glm::vec3(6);
  plane->materials[0].color = glm::vec3(0.2,0.2,0.85);
  plane->rotation.x = 90;
  plane->position.z = 0;

  //light
  std::shared_ptr<Light> light = make_shared<Light>();
  light->position = glm::vec3(0.8,3,6) * 5.0f;

  //camera
  std::shared_ptr<Camera> camera = make_shared<Camera>();
  camera->position = glm::vec3(0,0,4);
  camera->aspect_ratio = canvas->get_aspect_ratio();

  Transformable parent_transformable;
  
  shared_ptr<RayScene> ray_scene = make_shared<RayScene>();

  ray_scene->add_node(box);
  for(auto& s: spheres)
    ray_scene->add_node(s);
  ray_scene->add_node(plane);

  auto scene_renderer = make_shared<RaySceneRenderer>(frame_size, camera);
  scene_renderer->supersample_factor = supersample_factor;
  scene_renderer->light = light;
  
  cv::Mat image = scene_renderer->render_scene(ray_scene);
  
  cv::imshow("ray cast", image);
  cv::waitKey();
}
