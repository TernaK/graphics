#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics {
  template<typename T, GLenum target>
  struct VBO {
    GLuint vbo = -1;

    VBO() = default;

    VBO(T* data_ptr, int length, GLenum usage) {
      glGenBuffers(1, &vbo);
      glBindBuffer(target, vbo);
      glBufferData(target, length * sizeof(T), data_ptr, usage);
      glBindBuffer(target, 0);
    }

    void bind() {
      glBindBuffer(target, vbo);
    }

    void unbind() {
      glBindBuffer(target, 0);
    }

    void release() {
      glDeleteBuffers(1, &vbo);
      vbo = -1;
    }
  };
}
