set(libs_list ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw graphics_lib)

add_executable(hello hello.cpp)
target_link_libraries(hello ${libs_list})

add_executable(triangle triangle.cpp)
target_link_libraries(triangle ${libs_list})
