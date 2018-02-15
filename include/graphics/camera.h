#pragma once
#include <graphics/shader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

namespace graphics {
  struct Camera {
    glm::vec3 position = glm::vec3(0,2,5);
    GLfloat fov = 70.0;
    GLfloat aspect_ratio = 1.0;
    GLfloat z_near = 0.1;
    GLfloat z_far = 50.0;
    glm::vec3 target = glm::vec3(0);
    glm::vec3 up = glm::vec3(0,1,0);
    
    Camera() = default;
    glm::mat4 get_proj_mat() const;
    glm::mat4 get_view_mat() const;
    void set_uniforms(std::shared_ptr<Shader> shader) const;
  };
}
