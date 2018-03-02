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

bool ImplicitNode::hit_test(ray_t& ray, hit_t& hit, transform_t& transform,
                                Material& material) {
  int idx;
  bool did_hit = hit_test_idx(ray, hit, transform, idx);
  if(did_hit)
    material = materials[idx];
  return did_hit;
}

bool ImplicitNode::hit_test_idx(ray_t& ray, hit_t& hit, transform_t& transform, int& idx) {
  vector<tuple<bool,hit_t,int>> shape_hits = vector<tuple<bool,hit_t,int>>(shapes.size());
  for(int i = 0; i < shapes.size(); i++) {
    auto& shape = shapes[i];
    std::get<2>(shape_hits[i]) = i;
    if(shape == ShapeType::plane) {
      std::get<0>(shape_hits[i]) = hit_test_plane(ray, std::get<1>(shape_hits[i]),
                                                  transform, glm::vec3(0,1,0));
      if(!std::get<0>(shape_hits[i]))
        std::get<0>(shape_hits[i]) = hit_test_plane(ray, std::get<1>(shape_hits[i]),
                                                    transform, glm::vec3(0,-1,0));

    }
    else if(shape == ShapeType::box) {
      std::get<0>(shape_hits[i]) = hit_test_box(ray, std::get<1>(shape_hits[i]), transform);
    }
    else if(shape == ShapeType::flat_sphere || shape == ShapeType::sphere) {
      std::get<0>(shape_hits[i]) = hit_test_sphere(ray, std::get<1>(shape_hits[i]), transform);
    }
  }

  auto end = std::remove_if(shape_hits.begin(), shape_hits.end(),
                            [](const tuple<bool,hit_t,int>& h) -> bool {
                              return std::get<0>(h) == false;
                            });
  shape_hits.resize(end - shape_hits.begin());
  std::sort(shape_hits.begin(), shape_hits.end(),
            [](const tuple<bool,hit_t,int>& h1, tuple<bool,hit_t,int>& h2) -> bool {
              return std::get<1>(h1).dist < std::get<1>(h2).dist;
            });
  if(!shape_hits.empty()) {
    hit = std::get<1>(shape_hits[0]);
    idx = std::get<2>(shape_hits[0]);
    return true;
  } else {
    return false;
  }
}
