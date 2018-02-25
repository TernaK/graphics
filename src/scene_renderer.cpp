#include <graphics/scene_renderer.h>
#include <iostream>
using namespace std;
using namespace graphics;

SceneRenderer::SceneRenderer() {
  std::shared_ptr<Shader> object3d_shader = Shader::make_object3d_point_shader();
  shaders[object3d_shader->name] = object3d_shader;
  std::shared_ptr<Shader> mesh_shader = Shader::make_mesh_point_shader();
  shaders[mesh_shader->name] = object3d_shader;
  std::shared_ptr<Shader> sprite_shader = Shader::make_sprite_shader();
  shaders[sprite_shader->name] = object3d_shader;
}

void SceneRenderer::traverse_node(std::shared_ptr<SceneNode> node,
                                  transform_t parent_transform,
                                  std::map<std::shared_ptr<Shader>,shader_group_t>& groups) {

  transform_t this_transform = node->get_transform(parent_transform.model);
  for(auto& child: node->children)
    traverse_node(child, this_transform, groups);

//  if(node->requires_shader) {
    std::string shader_name = node->get_shader_name();
    auto shader_ptr = shaders.find(shader_name);
    if(shader_ptr == shaders.end())
      throw runtime_error("shader: shader_name not found");

    if(node->light)
      groups[shader_ptr->second].lights.insert(node->light);

    groups[shader_ptr->second].nodes_trans.push_back({node, parent_transform});
//  }
}

std::map<std::shared_ptr<Shader>, shader_group_t>
SceneRenderer::traverse_scene(std::shared_ptr<Scene> scene) {
  std::map<std::shared_ptr<Shader>, shader_group_t> groups;
  traverse_node(scene->root, transform_t(), groups);

  for(auto& group: groups) {
    if(!group.second.nodes_trans.empty())
      if(group.second.nodes_trans[0].node->requires_camera)
        group.second.camera = scene->camera;
  }

  return groups;
}

void SceneRenderer::render_scene(std::shared_ptr<Scene> scene) {
  auto groups = traverse_scene(scene);
  for(auto& group: groups) {
    auto& shader = group.first;
    shader->use();
    if(group.second.camera)
      group.second.camera->set_uniforms(shader);
    if(!group.second.lights.empty())
      group.second.lights.begin().operator*()->set_uniforms(shader);
    for(auto& node_trans: group.second.nodes_trans)
      node_trans.node->draw_node(shader, node_trans.trans.model);
  }
}

