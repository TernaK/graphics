#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>();
  Light light;
  Camera camera;
  camera.aspect_ratio = canvas->get_aspect_ratio();
  camera.position = glm::vec3(0,8,8);
  light.position.y = 10;
  light.type = LightType::directional;

  auto texture = make_shared<Texture>(TEXTURES_DIR + "/triangles.jpg");

  auto smooth_sphere_geometry = make_shared<Geometry>(ShapeType::terrain);
  auto smooth_sphere = make_shared<Object3DTex>(smooth_sphere_geometry, texture);
  smooth_sphere->name = "smooth_sphere";
  smooth_sphere->position = glm::vec3(0,0,0);
  smooth_sphere->scale = glm::vec3(3);
  smooth_sphere->material.color = glm::vec3(0.1, 0.2, 0.6);
  smooth_sphere->material.shininess = 256;
  smooth_sphere->material.strength.y = 0.9;
  smooth_sphere->material.shininess = 2;
  auto shader = smooth_sphere->shader;

  while(canvas->still_open()) {
    canvas->clear();

    shader->use();

    light.set_uniforms(shader);
    camera.set_uniforms(shader);

    smooth_sphere->rotation += glm::vec3(0.3,0.1,0.2);

    smooth_sphere->draw();

    canvas->poll_events();
    canvas->swap_buffers();
  }
}
