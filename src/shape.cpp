#include <graphics/shape.h>

using namespace std;
using namespace graphics;

std::shared_ptr<Solid> SolidShape::make_cuboid(GLfloat x, GLfloat y, GLfloat z) {
  x = x/2;
  y = y/2;
  z = z/2;
  std::vector<glm::vec3> vertices = {
    glm::vec3(-x,y,z), glm::vec3(x,y,z),      //front top left 0, top right 1
    glm::vec3(-x,-y,z), glm::vec3(x,-y,z),    //front low left 2, low right 3
    glm::vec3(x,y,-z), glm::vec3(-x,y,-z),    //back top left 4, top right 5
    glm::vec3(x,-y,-z), glm::vec3(-x,-y,-z),  //back low left 6, low right 7
  };
  std::vector<GLint> indices = {
    0,1,2, 1,3,2, //front
    4,5,6, 5,7,6, //back
    5,0,7, 0,2,7, //left
    1,4,3, 4,6,3, //right
    5,4,0, 4,1,0, //top
    2,3,7, 3,6,7  //bottom
  };
  shared_ptr<graphics::Solid> solid = std::make_shared<graphics::Solid>(vertices, indices);
  return solid;
}

std::shared_ptr<Solid> SolidShape::make_cube(GLfloat size) {
  return make_cuboid(size, size, size);
}


std::shared_ptr<Solid> SolidShape::make_sphere(GLfloat r, int st, int sc) {
  if(st < 2 || sc < 3)
    throw runtime_error("cannot have < 2 stacks or < 3 slices");
  std::vector<glm::vec3> vertices;
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
    vertices.push_back(temp[0][0]);
    vertices.push_back(temp[1][(j + 1) % temp[1].size()]);
    vertices.push_back(temp[1][j]);
  }
  
  //mid
  for(int i = 1; i < temp.size() - 2; i++) {
    for(int j = 0; j < temp[i].size(); j++) {
      vertices.push_back(temp[i][j]);
      vertices.push_back(temp[i + 1][(j + 1) % temp[i].size()]);
      vertices.push_back(temp[i + 1][j]);

      vertices.push_back(temp[i][j]);
      vertices.push_back(temp[i][(j + 1) % temp[i].size()]);
      vertices.push_back(temp[i + 1][(j + 1) % temp[i].size()]);
    }
  }
  
  //bottom
  int pen_idx = temp.size() - 2;
  for(int j = 0; j < temp[pen_idx].size(); j++) {
    vertices.push_back(temp.back()[0]);
    vertices.push_back(temp[pen_idx][j]);
    vertices.push_back(temp[pen_idx][(j + 1) % temp[pen_idx].size()]);
  }
  
  shared_ptr<graphics::Solid> solid = std::make_shared<graphics::Solid>(vertices);
  return solid;
}
