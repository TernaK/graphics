#include <graphics/primitive.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Transform
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

// Primitive
//--------------------------------------------------------------------------------
Primitive::Primitive(PrimitiveType type)
: type(type) {
  make_primitive();
}

Primitive::Primitive(PrimitiveType type, params_t params)
: type(type), params(params) {
  make_primitive();
}

void Primitive::make_plane() {
  GLfloat h = 1.0;

  auto fl = glm::vec3(-h,0,h);   //front left
  auto fr = glm::vec3(h,0,h);    //front right
  auto bl = glm::vec3(h,0,-h);  //back left
  auto br = glm::vec3(-h,0,-h);   //back right

  positions = { br, bl, fl, fr };
  indices = {0,2,1, 1,2,3};
  init_from_positions();
}

void Primitive::make_box() {
  GLfloat h = 0.5;

  auto ftl = glm::vec3(-h,h,h);   //front top left
  auto ftr = glm::vec3(h,h,h);    //front top right
  auto fll = glm::vec3(-h,-h,h);  //front low left
  auto flr = glm::vec3(h,-h,h);   //front low right
  auto btl = glm::vec3(h,h,-h);   //back top left
  auto btr = glm::vec3(-h,h,-h);  //back top right
  auto bll = glm::vec3(h,-h,-h);  //back low left
  auto blr = glm::vec3(-h,-h,-h);  //back low right

  positions = {
    ftl, ftr, fll, flr, //front
    btl, btr, bll, blr, //back
    btr, ftl, blr, fll, //left
    ftr, btl, flr, bll, //right
    btr, btl, ftl, ftr, //top
    fll, flr, blr, bll  //bottom
  };

  indices = {0,2,1, 1,2,3};
  for(int i = 1 ; i < positions.size() / 4; i++)
    for(int j = 0; j < 6; j++)
      indices.push_back(indices[j] + i*4);

  init_from_positions();
}


void Primitive::make_flat_sphere(int st, int sc) {
  GLfloat r = 1.0;
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");

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

  init_from_positions();
}

void Primitive::make_smooth_sphere(int st, int sc) {
  GLfloat r = 1.0;
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");
  //  std::vector<glm::vec3> positions;
  //  std::vector<GLint> indices;

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

  init_from_facets();
}

void Primitive::make_primitive() {
  if(type == PrimitiveType::box)
    make_box();
  else if(type == PrimitiveType::smooth_sphere)
    make_smooth_sphere(params.stacks, params.slices);
  else if(type == PrimitiveType::flat_sphere)
    make_flat_sphere(params.stacks, params.slices);
  else if(type == PrimitiveType::plane)
    make_plane();
}

PrimitiveType Primitive::get_type() {
  return type;
}

bool Primitive::ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) {
  glm::vec3 p = glm::vec3(transform.model_inv * glm::vec4(ray.p, 1.0));
  glm::vec3 d = glm::normalize(glm::vec3(transform.normal_inv * glm::vec4(ray.d, 1.0)));
  
  if(type == PrimitiveType::plane) {
    float t = -glm::dot(glm::vec3(0,1,0), p) / glm::dot(glm::vec3(0,1,0), d);
    hit.p = ray.p + t * ray.d;
    glm::vec3 prim_xcept = p + t * d;
    auto test = glm::lessThan(glm::abs(prim_xcept), glm::vec3(1,RAYEPSILON,1));
    if(glm::all(test)) {
      glm::vec3 n = transform.normal_inv * glm::vec3(0,1,0);
      hit.n = glm::normalize(n);
      return true;
    }
  } else if(type == PrimitiveType::smooth_sphere) {
  } else if(type == PrimitiveType::flat_sphere) {
  } else if(type == PrimitiveType::box) {
  }
  return false;
}

