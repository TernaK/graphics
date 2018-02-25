#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace graphics;

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(const std::vector<std::shared_ptr<Mesh>>& _meshes)
: meshes(std::move(_meshes)) {

}

Geometry::Geometry(const std::vector<glm::vec3>& _positions,
                   const std::vector<GLuint>& _indices) {
  meshes = { make_shared<Mesh>(_positions, _indices) };
}

void Geometry::draw() {
  GLint old_state;
  glGetIntegerv(GL_POLYGON_MODE, &old_state);
  if(wire_frame)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  for(auto mesh: meshes)
    mesh->draw();

  glPolygonMode(GL_FRONT_AND_BACK, old_state);
}

std::shared_ptr<Geometry> Geometry::create_box(GLfloat x, GLfloat y, GLfloat z) {
  x = x/2;
  y = y/2;
  z = z/2;

  auto ftl = glm::vec3(-x,y,z);   //front top left
  auto ftr = glm::vec3(x,y,z);    //front top right
  auto fll = glm::vec3(-x,-y,z);  //front low left
  auto flr = glm::vec3(x,-y,z);   //front low right
  auto btl = glm::vec3(x,y,-z);   //back top left
  auto btr = glm::vec3(-x,y,-z);  //back top right
  auto bll = glm::vec3(x,-y,-z);  //back low left
  auto blr = glm::vec3(-x,-y,-z);  //back low right

  vector<vector<glm::vec3>> face_positions = {
    { ftl, ftr, fll, flr }, //front
    { btl, btr, bll, blr }, //back
    { btr, ftl, blr, fll }, //left
    { ftr, btl, flr, bll }, //right
    { btr, btl, ftl, ftr }, //top
    { fll, flr, blr, bll }  //bottom
  };

  std::vector<GLuint> indices = {0,2,1, 1,2,3};
  vector<shared_ptr<Mesh>> meshes;
  for(auto& positons: face_positions) {
    auto mesh = make_shared<Mesh>(positons, indices);
    meshes.push_back(mesh);
  }

  shared_ptr<Geometry> box = make_shared<Geometry>(meshes);
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

  vector<shared_ptr<Mesh>> meshes = { make_shared<Mesh>(positions) };
  shared_ptr<graphics::Geometry> sphere = make_shared<Geometry>(meshes);
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

  vector<shared_ptr<Mesh>> meshes = { make_shared<Mesh>(facets, positions) };
  shared_ptr<graphics::Geometry> sphere = make_shared<Geometry>(meshes);
  return sphere;
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

  vector<shared_ptr<Mesh>> meshes = { make_shared<Mesh>(facets, positions) };
  shared_ptr<graphics::Geometry> terrain_geometry = make_shared<Geometry>(meshes);
  return terrain_geometry;
}
