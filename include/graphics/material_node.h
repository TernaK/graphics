#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <numeric>

namespace graphics {
  struct Material {
    glm::vec3 color = glm::vec3(0.6, 0.2, 0.7);
    float alpha = 1.0;

    Material() = default;
    Material(glm::vec3 color, float alpha)
    : color(color), alpha(alpha) {

    }
  };

  class MaterialNode {
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
    
    MaterialNode() = default;
    MaterialNode(const std::vector<glm::vec3>& vertices,
                 std::vector<GLint> indices = {},
                 Material material = Material());
    ~MaterialNode();

    void bind_vertex_data();
    void release_vertex_data();
    glm::mat4 get_model_mat() const;
    void draw() const;
  };
}
