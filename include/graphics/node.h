#pragma once
#include <graphics/shader.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

namespace graphics {
  class Node {
    GLuint vao; //note: multiple vaos allow for multiple attribute formats
    GLuint vbo;
    std::vector<GLfloat> data;

    void pack_vertex_data(const std::vector<GLfloat>& vertices,
                          const std::vector<GLfloat>& colors);
    void pack_vertex_data(const std::vector<glm::vec3>& vertices,
                          const std::vector<glm::vec4>& colors);

  public:
    Node() = default;
    Node(const std::vector<GLfloat>& vertices,
         const std::vector<GLfloat>& colors);
    Node(const std::vector<glm::vec3>& vertices,
         const std::vector<glm::vec4>& colors);
    ~Node();

    void bind_vertex_data();
    void release_vertex_data();
    void render(const Shader& shader);
  };
}
