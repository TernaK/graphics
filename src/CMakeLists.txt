# graphics_lib library
include_directories(${CMAKE_SOURCE_DIR}/include)
set(graphics_srcs
  ${CMAKE_SOURCE_DIR}/include/graphics/util.h
  node.cpp ${CMAKE_SOURCE_DIR}/include/graphics/node.h
  shader.cpp ${CMAKE_SOURCE_DIR}/include/graphics/shader.h
)
set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw glm)
add_library(graphics_lib ${graphics_srcs})
target_link_libraries(graphics_lib ${libs_list})

add_subdirectory(apps)
