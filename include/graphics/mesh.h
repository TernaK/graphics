#pragma once
#include <graphics/shader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <numeric>

namespace graphics {
  class Mesh {
    GLuint vao;           //vertex array object
    GLuint vbo_vertices;  //vertex buffer object for vertices
    GLuint vbo_colors;    //vertex buffer object for colors
    GLuint vbo_normals;    //vertex buffer object for colors
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLfloat> normals;
//    ShaderType shader_type;

    ///Convert glm arrays to primitive type arrays and store
    ///@param vertices every three vertices should represent a
    /// triangle in clockwise order
    ///@param colors must be supplied as vertex colors
    void store_vertex_data(const std::vector<glm::vec3>& vertices,
                           const std::vector<glm::vec4>& colors,
                           const std::vector<GLint>& indices);

    ///Use the adjacent traingle sides to compute the normals and store
    void compute_store_normals();

  public:
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 position = glm::vec3(0,0,0); //degrees
    glm::vec3 scale = glm::vec3(1,1,1);
    
    Mesh();
    Mesh(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec4>& colors,
         std::vector<GLint> indices = {});
    ~Mesh();

    void bind_vertex_data();
    void release_vertex_data();
//    void init_shader_type();
    void set_uniforms(std::shared_ptr<Shader> shader) const;
    void draw(std::shared_ptr<Shader> shader) const;
    glm::mat4 get_model_mat() const;
  };
}