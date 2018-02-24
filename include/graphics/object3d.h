#pragma once
#include <graphics/buffer_object.h>
#include <graphics/drawable.h>
#include <graphics/material.h>
#include <graphics/geometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <numeric>
#include <list>

namespace graphics {
  class Object3D : public Drawable {
  public:
    std::shared_ptr<Geometry> geometry;
    Material material;
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    std::list<std::shared_ptr<Object3D>> children;
    std::string name = "object";
    
    Object3D() = default;

    Object3D(std::shared_ptr<Geometry> geometry, Material material = Material());

    Object3D(const std::vector<glm::vec3>& vertices,
          std::vector<GLuint> indices = {},
          Material material = Material());

    ~Object3D();

    void set_uniforms(std::shared_ptr<Shader> shader,
                      const glm::mat4& p_model,
                      const glm::mat3& p_model_n) const;

    glm::mat4 get_model_mat() const;

    ShaderType get_shader_type() const override;

    void draw(std::shared_ptr<Shader> shader,
              const glm::mat4& p_model = glm::mat4(1.0),
              const glm::mat3& p_model_n = glm::mat4(1.0)) const override;

    void add_child(std::shared_ptr<Object3D> child);

    void remove_child(std::shared_ptr<Object3D> child);

    std::vector<std::shared_ptr<Object3D>> get_children_with_name(std::string name);
  };
}
