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
    GLuint vao;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> normals_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> positions_vbo;
    BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER> indices_ebo;

  public:
    Geometry geometry;
    Material material;
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    
    Object3D();
    Object3D(Geometry& geometry, Material material = Material());
    Object3D(const std::vector<glm::vec3>& vertices,
          std::vector<GLuint> indices = {},
          Material material = Material());
    ~Object3D();

    
    void set_uniforms(std::shared_ptr<Shader> shader) const;
    void bind_vertex_data();
    void release_vertex_data();
    glm::mat4 get_model_mat() const;
    ShaderType get_shader_type() const override;
    void draw(std::shared_ptr<Shader> shader) const override;
  };
}
