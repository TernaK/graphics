set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw graphics_lib ${OpenCV_LIBRARIES})

add_executable(hello hello.cpp)
target_link_libraries(hello ${libs_list})

add_executable(triangle triangle.cpp)
target_link_libraries(triangle ${libs_list})

add_executable(sprite_background sprite_background.cpp)
target_link_libraries(sprite_background ${libs_list})

add_executable(terrain_geometry terrain_geometry.cpp)
target_link_libraries(terrain_geometry ${libs_list})

add_executable(rendered_scene rendered_scene.cpp)
target_link_libraries(rendered_scene ${libs_list})

add_executable(object_texture object_texture.cpp)
target_link_libraries(object_texture ${libs_list})

add_executable(post_fx post_fx.cpp)
target_link_libraries(post_fx ${libs_list})

add_executable(points points.cpp)
target_link_libraries(points ${libs_list})
