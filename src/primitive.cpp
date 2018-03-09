#include <graphics/primitive.h>
using namespace std;
using namespace graphics;

// PrimitiveMaker
//--------------------------------------------------
void PrimitiveMaker::make_sphere(std::vector<Vertex>& vertices, std::vector<Facet>& facets,
                                 int st, int sl, bool smooth) {

  vector<vector<Vertex>> grid(st+1);
  for(int vidx = 0; vidx <= st; vidx++) {
    for(int uidx = 0; uidx <= sl; uidx++) {
      float az = 2 * M_PI * uidx / sl;
      float el = M_PI_2 - M_PI * vidx / st;
      float u = uidx / float(sl);
      float v = (st - vidx) / float(st);
      Vertex vert;
      vert.v = glm::vec3(cos(el)*sin(az), sin(el), cos(el)*cos(az));
      vert.uv = glm::vec2(u,v);
      grid[vidx].push_back(std::move(vert));
    }
  }

  auto get_idx = [](int r, int c, int C) {
    return C*r + c;
  };

  auto create_flat_face = [&](int r, int c,
                             int& idx) {
    Vertex vert00 = grid[r][c];
    Vertex vert01 = grid[r][c+1];
    Vertex vert10 = grid[r+1][c];
    Vertex vert11 = grid[r+1][c+1];
    glm::vec3 normal = glm::normalize(glm::cross(vert10.v - vert00.v,
                                                 vert01.v - vert00.v));
    //triangle1
    vertices.push_back(vert00);
    vertices.back().vn = normal;
    vertices.push_back(vert01);
    vertices.back().vn = normal;
    vertices.push_back(vert10);
    vertices.back().vn = normal;

    //triangle2
    vertices.push_back(vert01);
    vertices.back().vn = normal;
    vertices.push_back(vert10);
    vertices.back().vn = normal;
    vertices.push_back(vert11);
    vertices.back().vn = normal;

    facets.emplace_back(idx, idx+1, idx+2);
    facets.emplace_back(idx+3, idx+4, idx+5);
    idx += 6;
  };

  if(smooth) {
    for(int r = 0; r <= st; r++) {
      for(int c = 0; c <= sl; c++) {
        vertices.push_back(grid[r][c]);
        vertices.back().vn = vertices.back().v;
        int idx = get_idx(r,c,sl+1);
        if(r < st && c < sl) {
          facets.emplace_back(idx, idx+1, idx+sl+1);
          facets.emplace_back(idx+1, idx+sl+1, idx+sl+2);
        }
      }
    }
  } else {
    int idx = 0;
    for(int c = 0; c < sl; c++) {
      Vertex tip = grid[0][0];
      Vertex vert10 = grid[1][c];
      Vertex vert11 = grid[1][c+1];
      glm::vec3 normal = glm::normalize(glm::cross(vert11.v - vert10.v,
                                                   vert10.v - tip.v));
      tip.vn = normal;
      vert10.vn = normal;
      vert11.vn = normal;
      vertices.push_back(tip);
      vertices.push_back(vert10);
      vertices.push_back(vert11);

      facets.emplace_back(idx, idx+1, idx+2);
      idx += 3;
    }
    for(int r = 1; r < st; r++) {
      for(int c = 0; c < sl; c++) {
        create_flat_face(r,c,idx);
      }
    }
    for(int c = 0; c < sl; c++) {
      Vertex tip = grid[st][0];
      Vertex vert00 = grid[st-1][c];
      Vertex vert01 = grid[st-1][c+1];
      glm::vec3 normal = glm::normalize(glm::cross(vert01.v - tip.v,
                                                   vert00.v - tip.v));
      tip.vn = normal;
      vert00.vn = normal;
      vert01.vn = normal;
      vertices.push_back(vert00);
      vertices.push_back(vert01);
      vertices.push_back(tip);

      facets.emplace_back(idx, idx+1, idx+2);
    }
  }
}

void PrimitiveMaker::make_plane(std::vector<Vertex>& vertices, std::vector<Facet>& facets) {
  vector<pair<glm::vec3,glm::vec2>> pos_tex {
    {glm::vec3(-1,0,-1), glm::vec2(0,1)},
    {glm::vec3(1,0,-1), glm::vec2(1,1)},
    {glm::vec3(-1,0,1), glm::vec2(0,0)},
    {glm::vec3(1,0,1), glm::vec2(1,0)}
  };

  for(int v = 0 ; v < pos_tex.size(); v++) {
    Vertex vertex;
    vertex.v = pos_tex[v].first;
    vertex.uv = pos_tex[v].second;
    vertex.vn = glm::vec3(0,1,0);
    vertices.push_back(std::move(vertex));
  }

  facets.emplace_back(0,2,1);
  facets.emplace_back(1,2,3);
}

void PrimitiveMaker::make_box(std::vector<Vertex>& vertices, std::vector<Facet>& facets) {
  auto ftl = glm::vec3(-1,1,1);   //front top left
  auto ftr = glm::vec3(1,1,1);    //front top right
  auto fll = glm::vec3(-1,-1,1);  //front low left
  auto flr = glm::vec3(1,-1,1);   //front low right
  auto btl = glm::vec3(1,1,-1);   //back top left
  auto btr = glm::vec3(-1,1,-1);  //back top right
  auto bll = glm::vec3(1,-1,-1);  //back low left
  auto blr = glm::vec3(-1,-1,-1);  //back low right

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
  vector<glm::vec2> tex = {
    glm::vec2(0,1),
    glm::vec2(1,1),
    glm::vec2(0,0),
    glm::vec2(1,0)
  };
  std::vector<int> indices = {0,2,1, 1,2,3};

  for(int f = 0 ; f < positions.size(); f++) {
    for(int v = 0; v < positions[f].size(); v++) {
      //set vertex data
      Vertex vertex;
      vertex.v = positions[f][v];
      vertex.vn = normals[f];
      vertex.uv = tex[v];
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
