# graphics_lib library
include_directories(${CMAKE_SOURCE_DIR}/include)
set(graphics_srcs
  ${CMAKE_SOURCE_DIR}/include/graphics/util.h
  node.cpp ${CMAKE_SOURCE_DIR}/include/graphics/node.h
  shader.cpp ${CMAKE_SOURCE_DIR}/include/graphics/shader.h
  camera.cpp ${CMAKE_SOURCE_DIR}/include/graphics/camera.h
  # renderer.cpp ${CMAKE_SOURCE_DIR}/include/graphics/renderer.h
  light.cpp ${CMAKE_SOURCE_DIR}/include/graphics/light.h
  solid.cpp ${CMAKE_SOURCE_DIR}/include/graphics/solid.h
  texture.cpp ${CMAKE_SOURCE_DIR}/include/graphics/texture.h
  sprite.cpp ${CMAKE_SOURCE_DIR}/include/graphics/sprite.h
  scene.cpp ${CMAKE_SOURCE_DIR}/include/graphics/scene.h
  canvas.cpp ${CMAKE_SOURCE_DIR}/include/graphics/canvas.h
  ${CMAKE_SOURCE_DIR}/include/graphics/material.h
)
set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw glm ${OpenCV_LIBRARIES})
add_library(graphics_lib ${graphics_srcs})
target_link_libraries(graphics_lib ${libs_list})

add_subdirectory(apps)
