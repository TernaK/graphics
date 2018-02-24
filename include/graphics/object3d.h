#pragma once
#include <graphics/buffer_object.h>
#include <graphics/drawable.h>
#include <graphics/material.h>
#include <graphics/geometry.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <numeric>

namespace graphics {
  class Object3D : public Drawable {
  public:
    std::shared_ptr<Geometry> geometry;
    Material material;
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    
    Object3D();

    Object3D(std::shared_ptr<Geometry> geometry, Material material = Material());

    Object3D(const std::vector<glm::vec3>& vertices,
          std::vector<GLuint> indices = {},
          Material material = Material());

    ~Object3D();

    void set_uniforms(std::shared_ptr<Shader> shader) const;

    glm::mat4 get_model_mat() const;

    ShaderType get_shader_type() const override;

    void draw(std::shared_ptr<Shader> shader) const override;
  };
}
