#include <graphics/camera.h>

using namespace graphics;
using namespace std;

glm::mat4 Camera::get_proj_mat() const {
  return glm::perspective(fov, aspect_ratio, z_near, z_far);
}

glm::mat4 Camera::get_view_mat() const {
  return glm::lookAt(position, target, up);
}

void Camera::set_uniforms(GLuint program) const {
  GLint loc = glGetUniformLocation(program, "_cam_pos");
  glUniform3fv(loc, 1, glm::value_ptr(position));

  loc = glGetUniformLocation(program, "_proj_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(get_proj_mat()));

  loc = glGetUniformLocation(program, "_view_mat");
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(get_view_mat()));
}
