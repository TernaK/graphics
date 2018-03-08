#include <graphics/primitive.h>
using namespace std;
using namespace graphics;

// PrimitiveMaker
//--------------------------------------------------
void PrimitiveMaker::make_sphere_grid(std::vector<vector<int>> &grid_indices,
                                      std::vector<glm::vec3> &positions,
                                      int sc, int st) {
  float r = 1.0;
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
    int32_t idx[] = {facet.a, facet.b, facet.c};
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
  float h = 1.0;

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
  float h = 1;

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

glm::vec3 PrimitiveMaker::get_facet_normal(const Facet& facet,
                                           const std::vector<Vertex>& vertices) {
  glm::vec3 pa = vertices[facet.a].v;
  glm::vec3 pb = vertices[facet.b].v;
  glm::vec3 pc = vertices[facet.c].v;
  glm::vec3 normal = glm::normalize(glm::cross(pc - pb, pa - pb));
  return normal;
}

void PrimitiveMaker::make_terrain(vector<Vertex>& vertices, vector<Facet>& facets,
                                  int z_len, int x_len) {

  vector<glm::vec3> positions;
  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      float noise0 = 0.3 * glm::perlin(glm::vec2(2*x_val, 3*z_val));
      float noise1 = 0.03 * glm::perlin(glm::vec2(10*x_val, 10*z_val));
      positions.push_back(glm::vec3(x_val, noise0 + noise1, z_val));
      Vertex vertex;
      vertex.v = glm::vec3(x_val, noise0 + noise1, z_val);
      vertices.push_back(vertex);
    }
  }
  
  auto find_idx = [](int z, int x, int X) -> int { return  X * z + x; };
  for(int z = 0; z < z_len - 1; z++) {
    for(int x = 0; x < x_len - 1; x++) {
      int a,b,c;
      //triangle 1
      a = find_idx(z,x, x_len);
      b = find_idx(z+1,x, x_len);
      c = find_idx(z+1,x+1, x_len);
      facets.push_back(Facet(a, b, c));
      //triangle 2
      a = find_idx(z,x, x_len);
      b = find_idx(z+1,x+1, x_len);
      c = find_idx(z,x+1, x_len);
      facets.push_back(Facet(a, b, c));
    }
  }

  compute_smooth_normals(vertices, facets);
}

void PrimitiveMaker::compute_smooth_normals(std::vector<Vertex>& vertices,
                                            std::vector<Facet>& facets){
  //each index represents a vertex index
  vector<vector<Facet*>> assoc(vertices.size());
  for(int i = 0; i < facets.size(); i++) {
    assoc[facets[i].a].push_back( &(facets[i]) );
    assoc[facets[i].b].push_back( &(facets[i]) );
    assoc[facets[i].c].push_back( &(facets[i]) );
  }
  for(int i = 0; i < assoc.size(); i++) {
    glm::vec3 normal;
    for(int j = 0; j < assoc[i].size(); j++) {
      normal += get_facet_normal( *(assoc[i][j]), vertices );
      normal /= float(assoc[i].size());
    }
    vertices[i].vn = normal;
  }
}
