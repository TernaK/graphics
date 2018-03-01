#include <graphics/mesh.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Mesh
//--------------------------------------------------------------------------------
Mesh::Mesh(std::vector<Facet> _facets,
           const std::vector<glm::vec3>& _positions,
           bool smooth,
           std::vector<glm::vec3> _normals,
           std::vector<glm::vec3> _tex_coords) :
facets(std::move(_facets)), positions(std::move(_positions)),
normals(std::move(_normals)), tex_coords(std::move(_tex_coords)) {
  init_from_facets();
}

Mesh::Mesh(const std::vector<glm::vec3>& _positions,
                   std::vector<GLuint> _indices,
                   bool smooth):
positions(std::move(_positions)), indices(std::move(_indices)) {
  init_from_positions();
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<Facet>& facets)
: vertices(std::move(vertices)), facets(std::move(facets)) {
  create_indices_from_facets();
  init_from_vertices();
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<Facet>& facets,
           primitive_params_t params)
: vertices(std::move(vertices)), facets(std::move(facets)) {
  create_indices_from_facets();
  init_from_vertices();
}

Mesh::Mesh(MeshType mesh_type, primitive_params_t params) : mesh_type(mesh_type) {
  if(mesh_type == MeshType::box)
    make_box(vertices, facets);
  else if(mesh_type == MeshType::plane)
    PrimitiveMaker::make_plane(vertices, facets);
  else if(mesh_type == MeshType::sphere)
    PrimitiveMaker::make_sphere(vertices, facets, params.stacks, params.slices, true);
  else if(mesh_type == MeshType::flat_sphere)
    PrimitiveMaker::make_sphere(vertices, facets, params.stacks, params.slices, false);

  create_indices_from_facets();
  init_from_vertices();
}

Mesh::~Mesh() {
  positions_vbo.release();
  vertices_vbo.release();
  normals_vbo.release();
  texcoords_vbo.release();
  indices_ebo.release();
}

void Mesh::init_from_vertices() {
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

void Mesh::init_from_facets() {
  create_indices_from_facets();
  if(normals.empty()) {
    compute_smooth_normals();
  }
  bind_vertex_data();
}

void Mesh::init_from_positions() {
  if(indices.empty()) {
    for(int i = 0; i < positions.size()/3; i++)
      facets.emplace_back(i*3, i*3 + 1, i*3 + 2);
    create_indices_from_facets();
  } else {
    for(int i = 0; i < indices.size()/3; i++)
      facets.emplace_back(indices[i*3], indices[i*3 + 1], indices[i*3 + 2]);
  }

  compute_smooth_normals();
  bind_vertex_data();
}

void Mesh::compute_smooth_normals() {
  vector<vector<Facet*>> assoc(positions.size());
  for(int i = 0; i < facets.size(); i++) {
    assoc[facets[i].a].push_back( &(facets[i]) );
    assoc[facets[i].b].push_back( &(facets[i]) );
    assoc[facets[i].c].push_back( &(facets[i]) );
  }
  normals = vector<glm::vec3>(positions.size());
  for(int i = 0; i < assoc.size(); i++) {
    glm::vec3 normal;
    for(int j = 0; j < assoc[i].size(); j++) {
      normal += get_facet_normal( *(assoc[i][j]) ) / float(assoc[i].size());
    }
    normals[i] = normal;
  }
}

void Mesh::create_indices_from_facets() {
  indices.clear();
  for(int i = 0; i < facets.size(); i++) {
    indices.push_back(facets[i].a);
    indices.push_back(facets[i].b);
    indices.push_back(facets[i].c);
  }
}

glm::vec3 Mesh::get_facet_normal(const Facet& facet) {
  glm::vec3 pa = positions[facet.a];
  glm::vec3 pb = positions[facet.b];
  glm::vec3 pc = positions[facet.c];
  glm::vec3 normal = glm::normalize(glm::cross(pc - pb, pa - pb));
  return normal;
}

void Mesh::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //positions
  positions_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(&(positions.front().x),
                                                         positions.size() * 3,
                                                         GL_STATIC_DRAW);
  positions_vbo.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  positions_vbo.unbind();
  //normals
  normals_vbo = BufferObject<GLfloat, GL_ARRAY_BUFFER>(&(normals.front().x),
                                                       normals.size() * 3,
                                                       GL_STATIC_DRAW);
  normals_vbo.bind();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(1);
  normals_vbo.unbind();
  //indices
  indices_ebo = BufferObject<GLuint, GL_ELEMENT_ARRAY_BUFFER>(indices.data(),
                                                              indices.size(),
                                                              GL_STATIC_DRAW);
  glBindVertexArray(0);
}

void Mesh::draw() {
  if(vao == 0) return;
  glBindVertexArray(vao);
  indices_ebo.bind();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  indices_ebo.unbind();
  glBindVertexArray(0);
}

bool Mesh::ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) {
  bool did_hit = false;
  if(mesh_type == MeshType::plane) {
    return hit_test_plane(ray, hit, transform, glm::vec3(0,1,0));
  } else if(mesh_type == MeshType::box) {
    return hit_test_box(ray, hit, transform);
  } else if(mesh_type == MeshType::flat_sphere || mesh_type == MeshType::sphere) {
  }

  return did_hit;
}
