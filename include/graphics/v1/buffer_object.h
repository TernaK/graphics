#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics {
  template<typename T, GLenum target>
  struct BufferObject {
    GLuint buffer_object = 0;

    BufferObject() = default;

    ~BufferObject() {
    }

    BufferObject(T* data_ptr, int length, GLenum usage) {
      glGenBuffers(1, &buffer_object);
      glBindBuffer(target, buffer_object);
      glBufferData(target, length * sizeof(T), data_ptr, usage);
      glBindBuffer(target, 0);
    }

    void bind() const {
      glBindBuffer(target, buffer_object);
    }

    void unbind() const {
      glBindBuffer(target, 0);
    }

    void release() {
      if(buffer_object == 0) return;
      glDeleteBuffers(1, &buffer_object);
      buffer_object = 0;
    }
  };
}
