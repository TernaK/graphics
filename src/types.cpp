#include <graphics/types.h>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace graphics;

// Transformable
//--------------------------------------------------------------------------------
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

// Facet
//--------------------------------------------------------------------------------
Facet::Facet(int32_t a, int32_t b, int32_t c)
: a(a), b(b), c(c) {
  indices[0] = &a;
  indices[1] = &b;
  indices[2] = &c;
}

int32_t Facet::operator[](int idx) {
  return *(indices[idx]);
}

// HitTester
//--------------------------------------------------------------------------------
bool HitTester::ray_hit_test(ray_t& ray, hit_t& hit, transform_t& transform) {
  return false;
}

bool HitTester::hit_test_plane(ray_t& ray, hit_t& hit, transform_t& transform,
                               glm::vec3 plane_normal, glm::vec3 offset) {
  bool did_hit = false;
  glm::vec3 plane_o = glm::vec3(transform.model[3]) + offset;
  glm::vec3 plane_n = glm::normalize(transform.normal * plane_normal);
  float dist;
  if(glm::intersectRayPlane(ray.p, ray.d, plane_o, plane_n, dist)) {
    hit.p = ray.p + dist * ray.d;
    hit.n = plane_n;
    hit.dist = dist;
    glm::vec3 plane_hit = glm::vec3(transform.model_inv * glm::vec4(hit.p - offset, 1.0));
    glm::vec3 diff = glm::abs(plane_hit);
    if( glm::max(diff.x, max(diff.y, diff.z) ) < 1 )
      did_hit = true;
  }
  return did_hit;
}

bool HitTester::hit_test_box(ray_t& ray, hit_t& hit, transform_t& transform) {
  vector<pair<bool, hit_t>> hit_pairs(6);
  static const vector<glm::vec3> normals = {
    glm::vec3(0,1,0), glm::vec3(0,-1,0),
    glm::vec3(-1,0,0), glm::vec3(1,0,0),
    glm::vec3(0,0,1), glm::vec3(0,0,-1),
  };
  for(int i = 0; i < 6; i++) {
    transform_t plane_transform = transform;
    glm::vec3 offset = glm::vec3(plane_transform.model * glm::vec4(normals[i], 1.0));
    hit_pairs[i].first = hit_test_plane(ray, hit_pairs[i].second, plane_transform, normals[i], offset);
  }
  auto end = std::remove_if(hit_pairs.begin(), hit_pairs.end(),
                            [](const pair<bool, hit_t>& h) -> bool {
                              return h.first == false;
                            });
  hit_pairs.resize(end - hit_pairs.begin());
  std::sort(hit_pairs.begin(), hit_pairs.end(),
            [](const pair<bool, hit_t>& h1, const pair<bool, hit_t>& h2) -> bool {
              return h1.second.dist < h2.second.dist;
            });
  if(!hit_pairs.empty()) {
    hit = hit_pairs[0].second;
    return true;
  } else {
    return false;
  }
}

bool HitTester::hit_test_sphere(ray_t& ray, hit_t& hit, transform_t& transform) {
  bool did_hit = false;
  glm::vec3 p = glm::vec3(transform.model_inv * glm::vec4(ray.p, 1.0));
  glm::vec3 d = glm::vec3(transform.normal_inv * glm::vec4(ray.d, 1.0));
  float a = dot(d,d);
  float b = 2 * dot(p,d);
  float c = dot(p,p) - 1;
  float det = b*b - 4*a*c;
  if(det > 0) {
    float sqrt_det = sqrt(det);
    float t1 = (-b + sqrt_det) / (2*a);
    float t2 = (-b - sqrt_det) / (2*a);
    float t = min(t1, t2);
    glm::vec3 hit_sphere = p + t * d;
    if(t > 0 && fabs(glm::length(hit_sphere) - 1) < RAYEPSILON) {
      did_hit = true;
      hit.p = ray.p + t * ray.d;
      hit.n = glm::normalize(transform.normal * hit_sphere);
      hit.dist = glm::length(hit.p - ray.p);
    }
  }
  return did_hit;
}
