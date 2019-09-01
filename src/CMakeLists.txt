# graphics_lib library
set(graphics_srcs
  graphics.cpp ${PROJECT_SOURCE_DIR}/include/graphics/graphics.h
  window.cpp ${PROJECT_SOURCE_DIR}/include/graphics/window.h
  shader.cpp ${PROJECT_SOURCE_DIR}/include/graphics/shader.h
  ${PROJECT_SOURCE_DIR}/include/graphics/buffer_object.h
  texture.cpp ${PROJECT_SOURCE_DIR}/include/graphics/texture.h
)

set(graphics_libs ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw glm ${OpenCV_LIBRARIES})

add_library(graphics_lib ${graphics_srcs})
target_link_libraries(graphics_lib PUBLIC ${graphics_libs})
target_include_directories(graphics_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(apps)
