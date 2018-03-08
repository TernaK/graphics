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
  light.ambient = glm::vec3(0.8);

  auto terrain_params = PrimitiveParams(50,50);
  auto terrain_geometry = make_shared<Geometry>(ShapeType::terrain, terrain_params);
  shared_ptr<Object3D> terrain = make_shared<Object3D>(terrain_geometry);
  terrain->geometry->wire_frame = true;
  terrain->scale = glm::vec3(5);
  terrain->position.z = 2;
  terrain->material.color = glm::vec3(0.1, 0.4, 0.8);
  terrain->material.shininess = 64;

  auto shader = Shader::make_object3d_shader();

  while(canvas->still_open()) {
    canvas->clear();

    shader->use();

    light.set_uniforms(shader);
    camera.set_uniforms(shader);

    terrain->rotation.y += 0.3;

    terrain->draw(shader);

    canvas->poll_events();
    canvas->swap_buffers();
  }
}
