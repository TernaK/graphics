#include <graphics/mesh.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Transformable
//--------------------------------------------------
void PrimitiveMaker::make_flat_sphere(std::vector<Vertex>& vertices, std::vector<Facet>& facets,
                                      int st, int sc) {
}

void PrimitiveMaker::make_sphere(std::vector<Vertex>& vertices, std::vector<Facet>& facets,
                                        int st, int sc) {
  GLfloat r = 1.0;
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");
  std::vector<glm::vec3> positions;
  std::vector<GLint> indices;

  vector<vector<int>> grid_indices(st + 1, {}); //position indices of grid points
  int accum = 0;

  //top section
  grid_indices.front().push_back(accum++);
  positions.emplace_back(0,r,0);
  //mid sections
  float per_st_angle = 180.0 / st;
  float per_sc_angle = 360.0 / sc;
  for(int i = 1; i < st; i++) {
    float st_angle = 90.0 - (per_st_angle * i);
    float sc_r = r * cos(glm::radians(st_angle));
    float y = r * sin(glm::radians(st_angle));
    for(int j = 0; j < sc; j++) {
      float sc_angle = per_sc_angle * j;
      float x = sc_r * sin(glm::radians(sc_angle));
      float z = sc_r * cos(glm::radians(sc_angle));

      positions.emplace_back(x,y,z);
      grid_indices[i].push_back(accum++);
    }
  }
  //low section
  grid_indices.back().push_back(accum++);
  positions.emplace_back(0,-r,0);

  //assign vertices and indices
  vector<bool> assigned = vector<bool>(positions.size(), false);
  auto assign_facet_positions = [&](Facet& facet) {
    GLuint idx[] = {facet.a, facet.b, facet.c};
    for(int i = 0; i < 3; i++) {
      if(assigned[idx[i]]) continue;
      assigned[idx[i]] = true;
      Vertex vert;
      vert.v = positions[idx[i]];
      vert.vn = vert.v;
      vertices.push_back(vert);
    }
  };

  //top
  Vertex v1, v2, v3;
  for(int j = 0; j < grid_indices[1].size(); j++) {
    int a = grid_indices[0][0];
    int b = grid_indices[1][j];
    int c = grid_indices[1][(j + 1) % grid_indices[1].size()];
    facets.emplace_back(a,b,c);
    assign_facet_positions(facets.back());
  }

  //mid
  for(int i = 1; i < grid_indices.size() - 2; i++) {
    for(int j = 0; j < grid_indices[i].size(); j++) {
      int a = grid_indices[i][j];
      int b = grid_indices[i + 1][j];
      int c = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      facets.emplace_back(a,b,c);
      assign_facet_positions(facets.back());

      int d = grid_indices[i][j];
      int e = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      int f = grid_indices[i][(j + 1) % grid_indices[i].size()];
      facets.emplace_back(d,e,f);
      assign_facet_positions(facets.back());
    }
  }

  //bottom
  int pen_idx = grid_indices.size() - 2;
  for(int j = 0; j < grid_indices[pen_idx].size(); j++) {
    int a = grid_indices[pen_idx][j];
    int b = grid_indices.back()[0];
    int c = grid_indices[pen_idx][(j + 1) % grid_indices[pen_idx].size()];
    facets.emplace_back(a,b,c);
    assign_facet_positions(facets.back());
  }
}

void PrimitiveMaker::make_plane(std::vector<Vertex>& vertices, std::vector<Facet>& facets) {
  GLfloat h = 1.0;

  auto fl = glm::vec3(-h,0,h);   //front left
  auto fr = glm::vec3(h,0,h);    //front right
  auto bl = glm::vec3(h,0,-h);  //back left
  auto br = glm::vec3(-h,0,-h);   //back right

  vector<glm::vec3> positions = {
    br, bl, fl, fr
  };

  for(int v = 0 ; v < positions.size(); v++) {
    Vertex vertex;
    vertex.v = positions[v];
    vertex.vn = glm::vec3(0,1,0);
    vertices.push_back(std::move(vertex));
  }

  facets.emplace_back(0,2,1);
  facets.emplace_back(1,2,3);
}

void PrimitiveMaker::make_box(std::vector<Vertex>& vertices, std::vector<Facet>& facets) {
  GLfloat h = 1;

  auto ftl = glm::vec3(-h,h,h);   //front top left
  auto ftr = glm::vec3(h,h,h);    //front top right
  auto fll = glm::vec3(-h,-h,h);  //front low left
  auto flr = glm::vec3(h,-h,h);   //front low right
  auto btl = glm::vec3(h,h,-h);   //back top left
  auto btr = glm::vec3(-h,h,-h);  //back top right
  auto bll = glm::vec3(h,-h,-h);  //back low left
  auto blr = glm::vec3(-h,-h,-h);  //back low right

  vector<vector<glm::vec3>> positions = {
    {ftl, ftr, fll, flr}, //front
    {btl, btr, bll, blr}, //back
    {btr, ftl, blr, fll}, //left
    {ftr, btl, flr, bll}, //right
    {btr, btl, ftl, ftr}, //top
    {fll, flr, blr, bll}  //bottom
  };
  vector<glm::vec3> normals = {
    glm::vec3(0,0,1), glm::vec3(0,0,-1),
    glm::vec3(-1,0,0), glm::vec3(1,0,0),
    glm::vec3(0,1,0), glm::vec3(0,-1,0)
  };
  std::vector<int> indices = {0,2,1, 1,2,3};

  for(int f = 0 ; f < positions.size(); f++) {
    for(int v = 0; v < positions[f].size(); v++) {
      //set vertex data
      Vertex vertex;
      vertex.v = positions[f][v];
      vertex.vn = normals[f];
      vertices.push_back(std::move(vertex));
    }
    //set facet data
    facets.emplace_back(indices[0] + f*4, indices[1] + f*4, indices[2] + f*4);
    facets.emplace_back(indices[3] + f*4, indices[4] + f*4, indices[5] + f*4);
  }
}

// Transformable
//--------------------------------------------------
transform_t Transformable::get_transform(glm::mat4 p_model) {
  transform_t transform;
  transform.model = glm::translate(glm::mat4(1.0), position);
  transform.model = glm::rotate(transform.model, glm::radians(rotation.x), glm::vec3(1,0,0));
  transform.model = glm::rotate(transform.model, glm::radians(rotation.y), glm::vec3(0,1,0));
  transform.model = glm::rotate(transform.model, glm::radians(rotation.z), glm::vec3(0,0,1));
  transform.model = glm::scale(transform.model, scale);
  transform.model = p_model * transform.model;
  transform.model_inv = glm::inverse(transform.model);
  transform.normal = glm::transpose(glm::inverse(glm::mat3(transform.model)));
  transform.normal_inv = glm::transpose(transform.normal);
  return transform;
}

// Facet
//--------------------------------------------------------------------------------
Facet::Facet(GLuint a, GLuint b, GLuint c)
: a(a), b(b), c(c) {
  indices[0] = &a;
  indices[1] = &b;
  indices[2] = &c;
}

GLuint Facet::operator[](int idx) {
  return *(indices[idx]);
}

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
    make_plane(vertices, facets);
  else if(mesh_type == MeshType::sphere)
    make_sphere(vertices, facets, params.stacks, params.slices);

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
