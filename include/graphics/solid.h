#pragma once
#include <graphics/shader.h>
#include <graphics/material.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <numeric>

namespace graphics {
  class Solid  {
    GLuint vao;           //vertex array object
    GLuint vbo_vertices;  //vertex buffer object for vertices
    GLuint vbo_normals;    //vertex buffer object for colors
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;

    ///Convert glm arrays to primitive type arrays and store
    ///@param vertices every three vertices should represent a
    /// triangle in clockwise order
    void store_vertex_data(const std::vector<glm::vec3>& vertices,
                           const std::vector<GLint>& indices);

    ///Use the adjacent traingle sides to compute the normals and store
    void compute_store_normals();

  public:
    Material material;
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    
    Solid();
    Solid(const std::vector<glm::vec3>& vertices,
          std::vector<GLint> indices = {},
          Material material = Material());
    ~Solid();

//    void init_shader_type() override;
    void set_uniforms(std::shared_ptr<Shader> shader) const;
    void bind_vertex_data();
    void release_vertex_data();
    glm::mat4 get_model_mat() const;
    void draw(std::shared_ptr<Shader> shader) const;
  };
}
