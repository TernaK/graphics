#include <graphics/scene_renderer.h>
#include <iostream>
using namespace std;
using namespace graphics;

void SceneRenderer::traverse_node(std::shared_ptr<SceneNode> node,
                                  Transform parent_transform,
                                  std::map<std::shared_ptr<Shader>,ShaderGroup>& groups) {
  Transform this_transform = node->get_transform(parent_transform.model);
  for(auto& child: node->children)
    traverse_node(dynamic_pointer_cast<SceneNode>(child), this_transform, groups);

  if(node->geometry) {
    if(!groups.count(node->shader)) {
      ShaderGroup group;
      groups[node->shader] = group;
    } else {
      groups[node->shader].renderables.push_back({node, parent_transform});
    }
  }
}

std::map<std::shared_ptr<Shader>, SceneRenderer::ShaderGroup>
SceneRenderer::traverse_scene(std::shared_ptr<Scene> scene) {
  std::map<std::shared_ptr<Shader>, ShaderGroup> groups;
  traverse_node(scene->root, Transform(), groups);

  for(auto& group: groups) {
    if(!group.second.renderables.empty())
      if(group.second.renderables[0].node->requires_camera)
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

    if(!scene->lights.empty()) {
      shader->set_uniform("_num_lights", (int)scene->lights.size());
      for(int i = 0; i < scene->lights.size(); i++)
        scene->lights.begin().operator*()->set_uniforms(shader, i);
    }

    for(auto& rendrable: group.second.renderables)
      rendrable.node->draw_node(rendrable.trans.model);
  }
}

