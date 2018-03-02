#include <graphics/camera.h>

using namespace graphics;
using namespace std;

glm::mat4 Camera::get_proj_mat() const {
  return glm::perspective(fovy, aspect_ratio, z_near, z_far);
}

glm::mat4 Camera::get_view_mat() const {
  return glm::lookAt(position, target, up);
}

void Camera::set_uniforms(std::shared_ptr<Shader> shader) const {
  shader->set_uniform("_cam_pos", position);
  shader->set_uniform("_proj_mat", get_proj_mat());
  shader->set_uniform("_view_mat", get_view_mat());
}
