#include <graphics/mesh.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

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

// PrimitiveMaker
//--------------------------------------------------
void PrimitiveMaker::make_sphere_grid(std::vector<vector<int>> &grid_indices,
                                      std::vector<glm::vec3> &positions,
                                      int sc, int st) {
  GLfloat r = 1.0;
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");


  grid_indices = vector<vector<int>>(st + 1, {}); //position indices of grid points
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
}

void PrimitiveMaker::make_flat_sphere(std::vector<Vertex>& vertices, std::vector<Facet>& facets,
                                      int st, int sc) {
  vector<vector<int>> grid_indices;
  std::vector<glm::vec3> positions;
  make_sphere_grid(grid_indices, positions, sc, st);
}

void PrimitiveMaker::make_sphere(std::vector<Vertex>& vertices, std::vector<Facet>& facets,
                                 int st, int sc, bool smooth) {
  vector<vector<int>> grid_indices;
  std::vector<glm::vec3> positions;
  make_sphere_grid(grid_indices, positions, sc, st);

  //assign vertices and indices
  vector<bool> assigned = vector<bool>(positions.size(), false);
  auto assign_smooth_facet_vertices = [&](Facet& facet) {
    GLuint idx[] = {facet.a, facet.b, facet.c};
    for(int i = 0; i < 3; i++) {
      if(assigned[idx[i]]) continue;
      assigned[idx[i]] = true;
      Vertex vert;
      vert.v = positions[idx[i]];
      vert.vn = vert.v;
      vertices.push_back(vert);
    }
    facets.push_back(facet);
  };

  int flat_idx = 0;
  auto assign_flat_facet_vertices = [&](Facet& fake_facet) {
    glm::vec3 facet_v[3] =  {
      positions[fake_facet.a], positions[fake_facet.b], positions[fake_facet.c]
    };
    glm::vec3 normal = glm::normalize(glm::cross(facet_v[2] - facet_v[1], facet_v[0] - facet_v[1]));
    for(int i = 0; i < 3; i++) {
      Vertex vert;
      vert.v = facet_v[i];
      vert.vn = normal;
      vertices.push_back(vert);
    }
    facets.emplace_back(flat_idx, flat_idx+1, flat_idx+2);
    flat_idx += 3;
  };

  auto assign_facet = [&](Facet& facet) {
    if(smooth)
      assign_smooth_facet_vertices(facet);
    else
      assign_flat_facet_vertices(facet);
  };

  //top
  Vertex v1, v2, v3;
  for(int j = 0; j < grid_indices[1].size(); j++) {
    int a = grid_indices[0][0];
    int b = grid_indices[1][j];
    int c = grid_indices[1][(j + 1) % grid_indices[1].size()];
    Facet facet(a,b,c);
    assign_facet(facet);
  }

  //mid
  for(int i = 1; i < grid_indices.size() - 2; i++) {
    for(int j = 0; j < grid_indices[i].size(); j++) {
      int a = grid_indices[i][j];
      int b = grid_indices[i + 1][j];
      int c = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      Facet facet(a,b,c);
      assign_facet(facet);

      int d = grid_indices[i][j];
      int e = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      int f = grid_indices[i][(j + 1) % grid_indices[i].size()];
      facet = Facet(d,e,f);
      assign_facet(facet);
    }
  }

  //bottom
  int pen_idx = grid_indices.size() - 2;
  for(int j = 0; j < grid_indices[pen_idx].size(); j++) {
    int a = grid_indices[pen_idx][j];
    int b = grid_indices.back()[0];
    int c = grid_indices[pen_idx][(j + 1) % grid_indices[pen_idx].size()];
    Facet facet(a,b,c);
    assign_facet(facet);
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
