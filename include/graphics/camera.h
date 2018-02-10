#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
  struct Camera {
    glm::vec3 position = glm::vec3(0,0,5);
    GLfloat fov = 70.0;
    GLfloat aspect_ratio = 1.0;
    GLfloat z_near = 0.1;
    GLfloat z_far = 50.0;
    glm::vec3 target = glm::vec3(0);
    glm::vec3 up = glm::vec3(0,1,0);
    
    Camera() = default;
    glm::mat4 get_proj_mat();
    glm::mat4 get_view_mat();
    void set_uniforms(GLuint program);
  };
}
