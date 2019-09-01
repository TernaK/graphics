#include <graphics/scene_node.h>

using namespace std;
using namespace graphics;
// SceneNode
//--------------------------------------------------
SceneNode::SceneNode(std::shared_ptr<Geometry> _geometry)
: geometry(_geometry) {

}

void SceneNode::set_uniforms(Transform transform) {
  shader->set_uniform("_model_mat", transform.model);
  shader->set_uniform("_normal_mat", transform.normal);
}

void SceneNode::draw(glm::mat4 p_model) {
  if(!children.empty() && !hidden) {
    Transform transform = get_transform(p_model);
    draw_children(transform.model);//forward this model
  }
  draw_node(p_model);//forward parent model
}

void SceneNode::draw_node(glm::mat4 p_model) {
  if(hidden || !geometry) return;
  Transform transform = get_transform(p_model);
  set_uniforms(transform);
  geometry->draw();
}

void SceneNode::draw_children(glm::mat4 p_model) {
  for_each(children.begin(), children.end(), [&](const shared_ptr<SceneNode>& x) {
    x->draw(p_model);
  });
}
