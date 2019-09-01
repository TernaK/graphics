#pragma once
#include "graphics.h"

namespace graphics {
  //--------------------------------------------------BufferObject
  /// hold device data
  template<GLenum target, typename T = GLfloat>
  struct BufferObject {
    GLuint buffer_object = 0;

    BufferObject() = default;

    BufferObject(T* data_ptr, int length, GLenum usage = GL_STATIC_DRAW) {
      glGenBuffers(1, &buffer_object);
      glBindBuffer(target, buffer_object);
      glBufferData(target, length * sizeof(T), data_ptr, usage);
      glBindBuffer(target, 0);
    }

    ~BufferObject() = default;

    void bind() { glBindBuffer(target, buffer_object); }

    void unbind() { glBindBuffer(target, 0); }

    void release() {
      if(buffer_object == 0) return;
      glDeleteBuffers(1, &buffer_object);
      buffer_object = 0;
    }
  };
}
