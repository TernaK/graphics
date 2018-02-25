#pragma once
#include <graphics/light.h>
#include <graphics/geometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <list>

namespace graphics {
  const glm::mat4 MAT4EYE = glm::mat4(1.0);
  const glm::mat3 MAT3EYE = glm::mat3(1.0);
  const glm::vec3 VEC3EYE = glm::vec3(1.0);
  const glm::vec4 VEC4EYE = glm::vec4(1.0);
  
  struct transform_t {
    glm::mat4 model = MAT4EYE;
    glm::mat3 normal = MAT4EYE;
  };

  struct Transformable {
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);

    transform_t get_transform(glm::mat4 p_model = MAT4EYE);
  };

  struct SceneNode : Transformable {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Light> light;
    bool hidden = false;
    bool requires_shader = false;
    bool requires_camera = false;
    std::string name = "node";
    std::list<std::shared_ptr<SceneNode>> children;

    SceneNode(std::shared_ptr<Geometry> geometry);

    virtual void set_uniforms(std::shared_ptr<Shader> shader, transform_t transform);

    virtual std::string get_shader_name() = 0;

    virtual void draw(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);

    /// Calculate node's transform and premultiply by parent transform
    virtual void draw_node(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);

    virtual void draw_children(std::shared_ptr<Shader> shader, glm::mat4 p_model = MAT4EYE);

    void add_child(std::shared_ptr<SceneNode> child);

    void remove_child(std::shared_ptr<SceneNode> child);

    std::vector<std::shared_ptr<SceneNode>> get_children_with_name(std::string name);
  };
}
