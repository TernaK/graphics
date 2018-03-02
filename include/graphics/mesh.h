#pragma once
#include <graphics/buffer_object.h>
#include <graphics/primitive.h>

namespace graphics {
  
  struct Mesh : public HitTester, public PrimitiveMaker {
    std::vector<Vertex> vertices;
    std::vector<Facet> facets;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tex_coords;
    std::vector<GLuint> indices;
    ShapeType shape_type = ShapeType::undefined;

    GLuint vao = 0;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> vertices_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> positions_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> normals_vbo;
    BufferObject<GLfloat, GL_ARRAY_BUFFER> texcoords_vbo;
    BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER> indices_ebo;

    Mesh() = default;

    Mesh(std::vector<Facet> facets,
         const std::vector<glm::vec3>& positions,
         bool smooth = true,
         std::vector<glm::vec3> normals = {},
         std::vector<glm::vec3> tex_coords = {});

    Mesh(const std::vector<glm::vec3>& positions,
         std::vector<GLuint> indices = {},
         bool smooth = true);

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<Facet>& facets);

    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<Facet>& facets,
         primitive_params_t params);

    Mesh(ShapeType shape_type, primitive_params_t params = primitive_params_t());

    ~Mesh();

    void init_from_facets();

    void init_from_positions();

    void init_from_vertices();

    void bind_vertex_data();

    void compute_smooth_normals();

    void create_indices_from_facets();

    glm::vec3 get_facet_normal(const Facet& facet);

    void draw();

    bool ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) override;
  };
}
