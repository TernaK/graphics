# graphics_lib library
include_directories(${CMAKE_SOURCE_DIR}/include)
set(graphics_srcs
  canvas.cpp ${CMAKE_SOURCE_DIR}/include/graphics/canvas.h
  shader.cpp ${CMAKE_SOURCE_DIR}/include/graphics/shader.h
  light.cpp ${CMAKE_SOURCE_DIR}/include/graphics/light.h
  camera.cpp ${CMAKE_SOURCE_DIR}/include/graphics/camera.h
  mesh.cpp ${CMAKE_SOURCE_DIR}/include/graphics/mesh.h
  material.cpp ${CMAKE_SOURCE_DIR}/include/graphics/material.h
  texture.cpp ${CMAKE_SOURCE_DIR}/include/graphics/texture.h
  sprite.cpp ${CMAKE_SOURCE_DIR}/include/graphics/sprite.h
  drawable.cpp ${CMAKE_SOURCE_DIR}/include/graphics/drawable.h
  framebuffer.cpp ${CMAKE_SOURCE_DIR}/include/graphics/framebuffer.h
  geometry.cpp ${CMAKE_SOURCE_DIR}/include/graphics/geometry.h
  ${CMAKE_SOURCE_DIR}/include/graphics/buffer_object.h
  object3d.cpp ${CMAKE_SOURCE_DIR}/include/graphics/object3d.h
  mesh.cpp ${CMAKE_SOURCE_DIR}/include/graphics/mesh.h
  scene_node.cpp ${CMAKE_SOURCE_DIR}/include/graphics/scene_node.h
  scene_renderer.cpp ${CMAKE_SOURCE_DIR}/include/graphics/scene_renderer.h
  scene.cpp ${CMAKE_SOURCE_DIR}/include/graphics/scene.h
  ${CMAKE_SOURCE_DIR}/include/graphics/octree.h
  primitive.cpp ${CMAKE_SOURCE_DIR}/include/graphics/primitive.h
  types.cpp ${CMAKE_SOURCE_DIR}/include/graphics/types.h
)
set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw glm ${OpenCV_LIBRARIES})
add_library(graphics_lib ${graphics_srcs})
target_link_libraries(graphics_lib ${libs_list})

add_subdirectory(apps)
