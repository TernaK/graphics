#include <graphics/geometry.h>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace graphics;

// Geometry
//--------------------------------------------------------------------------------
Geometry::Geometry(const std::vector<std::shared_ptr<Mesh>>& _meshes)
: meshes(std::move(_meshes)) {

}

Geometry::Geometry(ShapeType mesh_type, PrimitiveParams params) {
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
