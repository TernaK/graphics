#pragma once
#include <graphics/light.h>
#include <graphics/geometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <list>

namespace graphics {
  struct SceneNode : public Transformable, public Inheritable<SceneNode> {
    std::shared_ptr<Geometry> geometry;
    bool hidden = false;
    bool requires_camera = false;

    SceneNode() = default;

    SceneNode(std::shared_ptr<Geometry> geometry);

    virtual void set_uniforms(std::shared_ptr<Shader> shader, Transform transform);

    virtual std::string get_shader_name() { return "object3d"; };

    virtual void draw(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);

    /// Calculate node's transform and premultiply by parent transform
    virtual void draw_node(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);

    virtual void draw_children(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);
  };
}
