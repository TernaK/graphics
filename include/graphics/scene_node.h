#pragma once
#include <graphics/light.h>
#include <graphics/geometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <list>

namespace graphics {
  struct SceneNode : public Transformable, public Inheritable<SceneNode> {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Shader> shader;
    bool hidden = false;
    bool requires_camera = false;

    SceneNode() = default;

    SceneNode(std::shared_ptr<Geometry> geometry);

    virtual void set_uniforms(Transform transform);

    virtual void draw(glm::mat4 p_model = MAT4EYE);

    virtual void draw_node(glm::mat4 p_model = MAT4EYE);

    virtual void draw_children(glm::mat4 p_model = MAT4EYE);
  };
}
