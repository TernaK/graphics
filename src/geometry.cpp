#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

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

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(std::vector<Facet> _facets,
                   const std::vector<glm::vec3>& _positions,
                   bool smooth,
                   std::vector<glm::vec3> _normals,
                   std::vector<glm::vec3> _tex_coords) :
facets(std::move(_facets)), positions(std::move(_positions)),
normals(std::move(_normals)), tex_coords(std::move(_tex_coords)) {
  create_indices_from_facets();
  if(_normals.empty()) {
    compute_smooth_normals();
  }
}

Geometry::Geometry(const std::vector<glm::vec3>& _positions,
                   std::vector<GLuint> _indices,
                   bool smooth):
positions(std::move(_positions)), indices(_indices) {
  if(_indices.empty()) {
    for(int i = 0; i < positions.size()/3; i++)
      facets.emplace_back(i*3, i*3 + 1, i*3 + 2);
    create_indices_from_facets();
  } else {
    for(int i = 0; i < indices.size()/3; i++)
      facets.emplace_back(indices[i*3], indices[i*3 + 1], indices[i*3 + 2]);
  }

  compute_smooth_normals();
}

void Geometry::compute_smooth_normals() {
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

const Geometry& Geometry::operator+=(const Geometry& r) {
  auto max = std::max_element(indices.begin(), indices.end());
  int offset = (max == indices.end()) ? 0 : *max + 1;
  for(int i = 0; i < r.facets.size(); i++)
    facets.emplace_back( r.facets[i].a + offset, r.facets[i].b + offset, r.facets[i].c + offset);
  for(int i = 0; i < r.indices.size(); i++)
    indices.emplace_back(r.indices[i] + offset);
  positions.insert(positions.end(), r.positions.begin(), r.positions.end());
  normals.insert(normals.end(), r.normals.begin(), r.normals.end());
  tex_coords.insert(tex_coords.end(), r.tex_coords.begin(), r.tex_coords.end());
  return *this;
}

void Geometry::create_indices_from_facets() {
  indices.clear();
  for(int i = 0; i < facets.size(); i++) {
    indices.push_back(facets[i].a);
    indices.push_back(facets[i].b);
    indices.push_back(facets[i].c);
  }
}

glm::vec3 Geometry::get_facet_normal(const Facet& facet) {
  glm::vec3 pa = positions[facet.a];
  glm::vec3 pb = positions[facet.b];
  glm::vec3 pc = positions[facet.c];
  glm::vec3 normal = glm::normalize(glm::cross(pc - pb, pa - pb));
  return normal;
}

cv::Mat make_random(cv::Size size) {
  cv::Mat mat = cv::Mat(size, CV_32F);
  mat.forEach<float>([](float& val, const int* pos) {
    val = abs( (float(rand() % 512) / 256.0) - 1.0 );
  });
  return mat;
}

float blerp(float p00, float p10, float p01, float p11,
            float x, float y) {
  x = glm::smoothstep(0.0f, 1.0f, x);
  y = glm::smoothstep(0.0f, 1.0f, y);
  float y0 = p00 + x * (p10 - p00);
  float y1 = p01 + x * (p11 - p01);
  return y0 + y * (y1 - y0);
}

cv::Mat make_value_noise(cv::Size size, cv::Mat seed) {
  cv::Mat result = cv::Mat(size, CV_32F);
  result.forEach<float>([&size, &seed](float& val, const int* rc) {
    cv::Point2f loc(float(rc[1])/size.width * (seed.cols - 1),
                    float(rc[0])/size.height * (seed.rows - 1));
    cv::Point l00(floor(loc.x), floor(loc.y));
    cv::Point l10 = l00 + cv::Point(1,0);
    cv::Point l01 = l00 + cv::Point(0,1);
    cv::Point l11 = l00 + cv::Point(1,1);
    float p00 = seed.at<float>(l00);
    float p10 = seed.at<float>(l10);
    float p01 = seed.at<float>(l01);
    float p11 = seed.at<float>(l11);
    float x = glm::fract(loc.x);
    float y = glm::fract(loc.y);
    val = blerp(p00, p10, p01, p11, x, y);
  });
  return result;
}

std::shared_ptr<Geometry> Geometry::create_terrain(int z_len, int x_len) {
  cv::Mat terrain = cv::Mat(z_len, x_len, CV_32F);
  cv::Mat seed0 = make_random({3,3});
  cv::Mat seed1 = make_random({20,20});
  cv::Mat noise0 = make_value_noise(cv::Size(x_len,z_len), seed0);
  cv::Mat noise1 = make_value_noise(cv::Size(x_len,z_len), seed1);
  cv::Mat noise = noise0 + 0.03 * noise1;
  
  vector<glm::vec3> positions;
  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      positions.push_back(glm::vec3(x_val,noise.at<float>(z,x),z_val));
    }
  }
  
  vector<Facet> facets;
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
  return make_shared<Geometry>(facets, positions);
}

std::shared_ptr<Geometry> Geometry::create_box(GLfloat x, GLfloat y, GLfloat z) {
  x = x/2;
  y = y/2;
  z = z/2;
  
  std::vector<glm::vec3> positions = {
    glm::vec3(-x,y,z), glm::vec3(x,y,z),      //front top left 0, top right 1
    glm::vec3(-x,-y,z), glm::vec3(x,-y,z),    //front low left 2, low right 3
    glm::vec3(x,y,-z), glm::vec3(-x,y,-z),    //back top left 4, top right 5
    glm::vec3(x,-y,-z), glm::vec3(-x,-y,-z),  //back low left 6, low right 7
  };
  
  std::vector<std::vector<GLuint>> indices = {
    {0,2,1, 1,2,3}, //front
    {4,6,5, 5,6,7}, //back
    {5,7,0, 0,7,2}, //left
    {1,3,4, 4,3,6}, //right
    {5,0,4, 4,0,1}, //top
    {2,7,3, 3,7,6}  //bottom
  };
  
  shared_ptr<Geometry> box = std::make_shared<Geometry>();
  for(int i = 0; i < indices.size(); i++) {
    // get sub indices
    std::vector<GLuint> sub_indices = {0,2,1, 1,2,3};
    // get sub positions
    std::vector<glm::vec3> sub_positions;
    sub_positions.push_back(positions[ indices[i][0] ]);
    sub_positions.push_back(positions[ indices[i][2] ]);
    sub_positions.push_back(positions[ indices[i][1] ]);
    sub_positions.push_back(positions[ indices[i][5] ]);
    
    auto face_geometry = Geometry(sub_positions,sub_indices);
    *box += face_geometry;
  }
  return box;
}

std::shared_ptr<Geometry> Geometry::create_flat_sphere(GLfloat r, int st, int sc) {
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");
  std::vector<glm::vec3> positions;
  std::vector<GLint> indices;
  
  vector<vector<glm::vec3>> temp(st + 1, {});
  //top section
  temp.front().emplace_back(0,r,0);
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
      
      temp[i].emplace_back(x,y,z);
    }
  }
  //low section
  temp.back().emplace_back(0,-r,0);
  
  //assign vertices and indices
  //top
  for(int j = 0; j < temp[1].size(); j++) {
    positions.push_back(temp[0][0]);
    positions.push_back(temp[1][j]);
    positions.push_back(temp[1][(j + 1) % temp[1].size()]);
  }
  
  //mid
  for(int i = 1; i < temp.size() - 2; i++) {
    for(int j = 0; j < temp[i].size(); j++) {
      positions.push_back(temp[i][j]);
      positions.push_back(temp[i + 1][j]);
      positions.push_back(temp[i + 1][(j + 1) % temp[i].size()]);
      
      positions.push_back(temp[i][j]);
      positions.push_back(temp[i + 1][(j + 1) % temp[i].size()]);
      positions.push_back(temp[i][(j + 1) % temp[i].size()]);
    }
  }
  
  //bottom
  int pen_idx = temp.size() - 2;
  for(int j = 0; j < temp[pen_idx].size(); j++) {
    positions.push_back(temp[pen_idx][j]);
    positions.push_back(temp.back()[0]);
    positions.push_back(temp[pen_idx][(j + 1) % temp[pen_idx].size()]);
  }
  
  shared_ptr<graphics::Geometry> sphere = make_shared<Geometry>(positions);
  return sphere;
}

std::shared_ptr<Geometry> Geometry::create_smooth_sphere(GLfloat r, int st, int sc) {
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
  vector<Facet> facets;
  //top
  for(int j = 0; j < grid_indices[1].size(); j++) {
    int a = grid_indices[0][0];
    int b = grid_indices[1][j];
    int c = grid_indices[1][(j + 1) % grid_indices[1].size()];
    facets.emplace_back(a,b,c);
  }
  
  //mid
  for(int i = 1; i < grid_indices.size() - 2; i++) {
    for(int j = 0; j < grid_indices[i].size(); j++) {
      int a = grid_indices[i][j];
      int b = grid_indices[i + 1][j];
      int c = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      facets.emplace_back(a,b,c);
      
      int d = grid_indices[i][j];
      int e = grid_indices[i + 1][(j + 1) % grid_indices[i].size()];
      int f = grid_indices[i][(j + 1) % grid_indices[i].size()];
      facets.emplace_back(d,e,f);
    }
  }
  
  //bottom
  int pen_idx = grid_indices.size() - 2;
  for(int j = 0; j < grid_indices[pen_idx].size(); j++) {
    int a = grid_indices[pen_idx][j];
    int b = grid_indices.back()[0];
    int c = grid_indices[pen_idx][(j + 1) % grid_indices[pen_idx].size()];
    facets.emplace_back(a,b,c);
  }
  
  shared_ptr<graphics::Geometry> sphere = make_shared<Geometry>(facets, positions);
  return sphere;
}
