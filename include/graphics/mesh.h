#pragma once
#include <graphics/buffer_object.h>
#include <graphics/primitive.h>

namespace graphics {
  
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<GLuint> indices;

    GLuint vao = 0;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> vertices_vbo;
    BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER> indices_ebo;

    Mesh() = default;

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<Facet>& facets);

    Mesh(ShapeType shape_type, PrimitiveParams params = PrimitiveParams());

    ~Mesh();

    void init();

    void bind_vertex_data();

    void create_indices_from_facets();
    
    void draw();
  };
}
