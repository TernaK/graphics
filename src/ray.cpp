#include <graphics/ray.h>
using namespace std;
using namespace graphics;

void RayScene::add_node(std::shared_ptr<ImplicitNode> node) {
  root->add_child(node);
}

RaySceneRenderer::RaySceneRenderer(cv::Size frame_size, std::shared_ptr<Camera> camera)
: frame_size(frame_size), camera(camera) {
  
}

void RaySceneRenderer::traverse_node(std::shared_ptr<ImplicitNode> node,
                                     std::vector<traversed_node_t>& traversed_nodes,
                                     transform_t& p_transform) {
  auto transform = node->get_transform() << p_transform;
  if(node->shapes.size() > 0) {
    RaySceneRenderer::traversed_node_t traversed;
    traversed.node = node;
    traversed.transform = transform;
    traversed_nodes.push_back(traversed);
  }
  
  for(auto& child: node->children) {
    traverse_node(child, traversed_nodes, transform);
  }
}

std::vector<RaySceneRenderer::traversed_node_t>
RaySceneRenderer::traverse_scene(std::shared_ptr<RayScene> scene) {
  std::vector<RaySceneRenderer::traversed_node_t> traversed_nodes;
  transform_t transform;
  traverse_node(scene->root, traversed_nodes, transform);
  return traversed_nodes;
}

cv::Vec3f shade_frag(hit_t& hit, Material& material, shared_ptr<Camera> camera, shared_ptr<Light> light) {
  glm::vec3 l_vec = glm::normalize(light->position - hit.p);
  glm::vec3 v_vec = glm::normalize(camera->position - hit.p);
  float dist = glm::length(l_vec);
  float attenuation = 1.0/(light->attenuation.x +
                           light->attenuation.y * dist +
                           light->attenuation.z * dist * dist);

  glm::vec3 ambient, diffuse, specular;
  ambient = material.strength.x * light->ambient;
  float cos_t = 0;
  cos_t = glm::dot(l_vec, hit.n);
  cos_t = cos_t < 0 ? 0 : cos_t;
  diffuse = material.strength.y * light->color * cos_t;
  glm::vec3 r = glm::reflect(-l_vec, hit.n);
  float spec = glm::dot(r, v_vec);
  spec = spec < 0 ? 0 : spec;
  specular = material.strength.z * light->color * pow(spec, material.shininess);
  glm::vec3 color = (ambient + attenuation * (diffuse + specular)) * material.color;
  color = glm::clamp(color, 0.0f, 1.0f);
  cv::Vec3f frag = cv::Vec3f(color.b, color.g, color.r);
  return frag;
}

cv::Mat RaySceneRenderer::draw_scene(std::vector<RaySceneRenderer::traversed_node_t>& traversed_nodes) {
  cv::Mat image = cv::Mat(frame_size * supersample_factor, CV_32FC3, {clear_color.x, clear_color.y, clear_color.z});
  cv::Mat z_buffer = cv::Mat(frame_size * supersample_factor, CV_32F, cv::Scalar::all(numeric_limits<float>::max()));
  
  int width = image.cols;
  int height = image.rows;
  
  float y_near = camera->z_near * tan(glm::radians(camera->fovy/2.0));
  float x_near = camera->aspect_ratio * y_near;
  
  image.forEach<cv::Vec3f>([&](cv::Vec3f& frag, const int* row_col) {
    int row = row_col[0];
    int col = row_col[1];
    //TODO: find the correct vectors
    float ray_dx = ((2.0 * col - width) / width) * x_near;
    float ray_dy = -((2.0 * row - height) / height) * y_near;
    
    //make ray
    ray_t ray;
    ray.p = glm::vec3(0,0,10);
    ray.d = glm::normalize(glm::vec3(ray_dx, ray_dy, -camera->z_near));

    vector<implicit_test_t> tests;
    for(auto& traversed: traversed_nodes) {
      float& z_buffer_px = z_buffer.at<float>(row, col);
      implicit_test_t test = traversed.node->hit_test(ray, traversed.transform);
      
      if(test.did_hit && test.hit.dist < z_buffer_px) {
        z_buffer_px = test.hit.dist;
        glm::vec3 l_vec = glm::normalize(light->position - test.hit.p);
        glm::vec3 v_vec = glm::normalize(ray.p - test.hit.p);
        float light_dist = glm::length(light->position - test.hit.p);
        
        //light ray
        ray_t light_ray;
        light_ray.p = light->position;
        light_ray.d = -l_vec;
        bool is_in_light = true;
        for(auto& l_traversed: traversed_nodes) {
          implicit_test_t light_test = l_traversed.node->hit_test(light_ray, l_traversed.transform);
          if(light_test.did_hit && (light_dist - light_test.hit.dist) > RAYEPSILON) {
            is_in_light = false;
            break;
          }
        }

        if(is_in_light) {
          frag = shade_frag(test.hit, test.material, camera, light);//cv::Vec3f(color.b, color.g, color.r);
        } else {
          glm::vec3 color = test.material.strength.x * light->ambient * test.material.color;
          frag = cv::Vec3f(color.b, color.g, color.r);
        }
      }

    }
  });
  
  cv::GaussianBlur(image, image, cv::Size(3,3), 0.8);
  cv::resize(image, image, frame_size);
  return image;
}

cv::Mat RaySceneRenderer::render_scene(std::shared_ptr<RayScene> scene) {
  auto traversed = traverse_scene(scene);
  return draw_scene(traversed);
}
