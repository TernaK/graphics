#include <graphics/implicit.h>
using namespace std;
using namespace graphics;

ImplicitNode::ImplicitNode(ShapeType shape, Material material) {
  shapes.push_back(std::move(shape));
  materials.push_back(std::move(material));
}

ImplicitNode::ImplicitNode(std::vector<ShapeType> shapes,
                           std::vector<Material> materials)
: shapes(std::move(shapes)), materials(std::move(materials)) {

}

bool ImplicitNode::ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) {
  vector<pair<bool,hit_t>> shape_hits = vector<pair<bool,hit_t>>(shapes.size());
  for(int i = 0; i < shapes.size(); i++) {
    auto& shape = shapes[i];
    if(shape == ShapeType::plane) {
      shape_hits[i].first = hit_test_plane(ray, shape_hits[i].second, transform, glm::vec3(0,1,0));
    } else if(shape == ShapeType::box) {
      shape_hits[i].first = hit_test_box(ray, shape_hits[i].second, transform);
    } else if(shape == ShapeType::flat_sphere || shape == ShapeType::sphere) {
      shape_hits[i].first = hit_test_sphere(ray, shape_hits[i].second, transform);
    }
  }

  auto end = std::remove_if(shape_hits.begin(), shape_hits.end(),
                            [](const pair<bool, hit_t>& h) -> bool {
                              return h.first == false;
                            });
  shape_hits.resize(end - shape_hits.begin());
  std::sort(shape_hits.begin(), shape_hits.end(),
            [](const pair<bool, hit_t>& h1, const pair<bool, hit_t>& h2) -> bool {
              return h1.second.dist < h2.second.dist;
            });
  if(!shape_hits.empty()) {
    hit = shape_hits[0].second;
    return true;
  } else {
    return false;
  }
}
