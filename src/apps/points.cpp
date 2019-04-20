#include "graphics/point_cloud.h"
#include "graphics/canvas.h"
#include "graphics/camera.h"

void read_points_data(std::string points_path,
                      std::vector<glm::vec3>& points, std::vector<glm::vec4>& colors,
                      bool color_mode = false) {
  points = std::vector<glm::vec3>();
  colors = std::vector<glm::vec4>();
}

void read_cameras_data(std::string cameras_path,
                       std::vector<glm::vec3>& cameras, std::vector<glm::vec4>& colors) {
  cameras = std::vector<glm::vec3>();
  colors = std::vector<glm::vec4>();
}

int main(int argc, char* args[]) {
  graphics::Canvas canvas;
  glPointSize(5);

  graphics::Shader shader(graphics::SHADERS_DIR + "simple3d_vshader.glsl",
                          graphics::SHADERS_DIR + "simple3d_fshader.glsl");
  shader.add_uniform("_proj_mat");
  shader.add_uniform("_view_mat");
  shader.add_uniform("_model_mat");
  shader.add_attribute("_pos");
  shader.add_attribute("_color");

  graphics::Camera camera;
  camera.position = glm::vec3(0,10,3);
  shader.use();
  shader.set_uniform("_proj_mat", camera.get_proj_mat());
  shader.set_uniform("_view_mat", camera.get_view_mat());
  glm::mat4 model_mat = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1,0,0));
  shader.set_uniform("_model_mat", model_mat);

  std::vector<glm::vec3> points_vertices;
  std::vector<glm::vec4> points_colors;
  ::read_points_data("/Users/terna/Downloads/vo_map/points.vio", points_vertices, points_colors);
  std::vector<glm::vec3> cameras_vertices;
  std::vector<glm::vec4> cameras_colors;
  ::read_points_data("/Users/terna/Downloads/vo_map/keyframes.vio", cameras_vertices, cameras_colors);
  auto points = std::make_shared<graphics::PointCloud>(points_vertices, points_colors);
  auto cameras = std::make_shared<graphics::PointCloud>(cameras_vertices, cameras_colors);
  canvas.clear();
  points->draw();
  cameras->draw();

  while(canvas.still_open()) {
    canvas.poll_events();
    canvas.swap_buffers();
  }
  canvas.close_window();
}
