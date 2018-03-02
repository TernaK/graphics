#include <graphics/scene_node.h>

using namespace std;
using namespace graphics;
// SceneNode
//--------------------------------------------------
SceneNode::SceneNode(std::shared_ptr<Geometry> _geometry)
: geometry(_geometry) {

}

void SceneNode::set_uniforms(std::shared_ptr<Shader> shader, transform_t transform) {
  shader->set_uniform("_model_mat", transform.model);
  shader->set_uniform("_normal_mat", transform.normal);
}

void SceneNode::draw(std::shared_ptr<Shader> shader, glm::mat4 p_model) {
  if(!children.empty() && !hidden) {
    transform_t transform = get_transform(p_model);
    draw_children(shader, transform.model);//forward this model
  }
  draw_node(shader, p_model);//forward parent model
}

void SceneNode::draw_node(std::shared_ptr<Shader> shader, glm::mat4 p_model) {
  if(hidden || !geometry) return;
  transform_t transform = get_transform(p_model);
  set_uniforms(shader, transform);
  geometry->draw();
}

void SceneNode::draw_children(std::shared_ptr<Shader> shader, glm::mat4 p_model) {
  for_each(children.begin(), children.end(), [&](const shared_ptr<SceneNode>& x) {
    x->draw(shader, p_model);
  });
}
