#include <graphics/camera.h>

using namespace graphics;
using namespace std;

glm::mat4 Camera::get_proj_mat() {
  return glm::perspective(fov, aspect_ratio, z_near, z_far);
}

glm::mat4 Camera::get_view_mat() {
  return glm::lookAt(position, target, up);
}

