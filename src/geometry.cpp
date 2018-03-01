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

Geometry::Geometry(MeshType mesh_type, primitive_params_t params) {
  meshes = { make_shared<Mesh>(mesh_type, params) };
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

std::shared_ptr<Geometry> Geometry::create_terrain(int z_len, int x_len) {
  vector<glm::vec3> positions;
  for(int z = 0; z < z_len; z++) {
    for(int x = 0; x < x_len; x++) {
      float z_val = (float(z) / (z_len-1)) * 1.0 + -0.5;
      float x_val = (float(x) / (x_len-1)) * 1.0 + -0.5;
      float noise0 = 0.3 * glm::perlin(glm::vec2(2*x_val, 3*z_val));
      float noise1 = 0.03 * glm::perlin(glm::vec2(10*x_val, 10*z_val));
      positions.push_back(glm::vec3(x_val, noise0 + noise1, z_val));
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
