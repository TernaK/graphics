#include <graphics/scene.h>

using namespace std;
using namespace graphics;

// Scene
//__________________________________________________
Scene::Scene(std::shared_ptr<Canvas> _canvas)
: canvas(_canvas) {
  camera->aspect_ratio = canvas->get_aspect_ratio();
  camera->position = glm::vec3(0,5,10);
}

void Scene::add_node(std::shared_ptr<SceneNode> node) {
  root->add_child(node);
}

void Scene::clear() {
  glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
