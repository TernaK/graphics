#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <numeric>

namespace graphics {
  class Node {
    GLuint vao;           //vertex array object
    GLuint vbo_vertices;  //vertex buffer object for vertices
    GLuint vbo_colors;    //vertex buffer object for colors
    GLuint ebo;           //vertex buffer object for element array
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLint> indices;

    ///Convert glm arrays to primitive type arrays and store
    void store_vertex_data(const std::vector<glm::vec3>& vertices,
                           const std::vector<glm::vec4>& colors);
    ///Create array with values from 0 to (len - 1)
    void create_indices(int len);

  public:
    Node() = default;
    Node(const std::vector<GLfloat>& vertices,
         const std::vector<GLfloat>& colors,
         std::vector<int> indices = {});
    Node(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec4>& colors,
         std::vector<int> indices = {});
    ~Node();

    void bind_vertex_data();
    void release_vertex_data();
    void render();
  };
}
