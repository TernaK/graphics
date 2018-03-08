#include <graphics/mesh.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Mesh
//--------------------------------------------------------------------------------
Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<Facet>& facets)
: vertices(std::move(vertices)), facets(std::move(facets)) {
  init();
}

Mesh::Mesh(ShapeType shape_type, PrimitiveParams params){
  if(shape_type == ShapeType::box)
    make_box(vertices, facets);
  else if(shape_type == ShapeType::plane)
    PrimitiveMaker::make_plane(vertices, facets);
  else if(shape_type == ShapeType::sphere)
    PrimitiveMaker::make_sphere(vertices, facets, params.stacks, params.slices, true);
  else if(shape_type == ShapeType::flat_sphere)
    PrimitiveMaker::make_sphere(vertices, facets, params.stacks, params.slices, false);
  else if(shape_type == ShapeType::terrain)
    PrimitiveMaker::make_terrain(vertices, facets, params.stacks, params.slices);

  init();
}

void Mesh::init() {
  create_indices_from_facets();
  bind_vertex_data();
}

Mesh::~Mesh() {
  vertices_vbo.release();
  indices_ebo.release();
}

void Mesh::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //vertices
  vertices_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(&(vertices.front().v.x),
                                                        vertices.size() * sizeof(Vertex) / sizeof(GLfloat),
                                                        GL_STATIC_DRAW);
  vertices_vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  vertices_vbo.unbind();

  //indices
  indices_ebo = BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER>(indices.data(),
                                                              indices.size(),
                                                              GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void Mesh::create_indices_from_facets() {
  indices.clear();
  for(int i = 0; i < facets.size(); i++) {
    indices.push_back(facets[i].a);
    indices.push_back(facets[i].b);
    indices.push_back(facets[i].c);
  }
}

void Mesh::draw() {
  if(vao == 0) return;
  glBindVertexArray(vao);
  indices_ebo.bind();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  indices_ebo.unbind();
  glBindVertexArray(0);
}
