#pragma once
#include <graphics/solid.h>

namespace graphics {
  struct SolidShape {
    static std::shared_ptr<Solid> make_cuboid(GLfloat x = 1.0,
                                              GLfloat y = 1.0,
                                              GLfloat z = 1.0);
    
    static std::shared_ptr<Solid> make_cube(GLfloat size = 1.0);
    
    static std::shared_ptr<Solid> make_sphere(GLfloat radius = 0.5,
                                              int stacks = 10,
                                              int sections = 10);
  };
}
