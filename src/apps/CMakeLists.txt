set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw graphics_lib ${OpenCV_LIBRARIES})

# add_executable(hello hello.cpp)
# target_link_libraries(hello ${libs_list})

add_executable(triangle triangle.cpp)
target_link_libraries(triangle ${libs_list})

add_executable(cube cube.cpp)
target_link_libraries(cube ${libs_list})

# add_executable(material_cube material_cube.cpp)
# target_link_libraries(material_cube ${libs_list})
#
# add_executable(sprite_background sprite_background.cpp)
# target_link_libraries(sprite_background ${libs_list})
#
# add_executable(shapes shapes.cpp)
# target_link_libraries(shapes ${libs_list})
#
# add_executable(post_fx post_fx.cpp)
# target_link_libraries(post_fx ${libs_list})
