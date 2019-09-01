#pragma once
#include "graphics.h"

namespace graphics {
  //--------------------------------------------------Texture
  struct Texture {
    GLuint texture;
    int texture_unit { 0 };

    Texture() = default;

    /// create a texture from raw data
    Texture(unsigned char* data, int width, int height, bool bgr = true);

    /// activate the associated texture unit and bind to it
    void activate();
  };
}
