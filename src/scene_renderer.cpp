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

std::map<std::shared_ptr<Shader>, shader_group_t>
SceneRenderer::traverse_scene(std::shared_ptr<Scene> scene) {
  std::map<std::shared_ptr<Shader>, shader_group_t> groups;
  if(!scene->root)
    return groups;

  stack<node_trans_t> node_stack;
  node_stack.push({scene->root});

  while(!node_stack.empty()) {
    auto& stack_top = node_stack.top();
    if(stack_top.node->requires_shader) {
      std::string shader_name = stack_top.node->get_shader_name();
      auto shader_ptr = shaders.find(shader_name);
      if(shader_ptr == shaders.end())
        throw runtime_error("shader: shader_name not found");

      if(stack_top.node->light)
        groups[shader_ptr->second].lights.insert(stack_top.node->light);

      groups[shader_ptr->second].nodes_trans.push_back(stack_top);
    }

    transform_t parent_transform = node_stack.top().node->get_transform();
    for(auto& child: node_stack.top().node->children) {
      node_stack.push({child, parent_transform});
      continue;
    }

    node_stack.pop();
  }

  for(auto& group: groups) {
    if(!group.second.nodes_trans.empty())
      if(group.second.nodes_trans[0].node->requires_camera)
        group.second.camera = scene->camera;
  }
  return groups;
}

void SceneRenderer::render_scene(std::shared_ptr<Scene> scene) {
  auto groups = traverse_scene(scene);
}

