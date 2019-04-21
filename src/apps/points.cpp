#include "graphics/point_cloud.h"
#include "graphics/object3d.h"
#include "graphics/scene_renderer.h"
#include <string>

void read_points_data(std::string points_path,
                      std::vector<glm::vec3>& points, std::vector<glm::vec4>& colors,
                      bool color_mode = true) {
  std::ifstream file(points_path);
  std::string header;
  std::getline(file, header);
  points = std::vector<glm::vec3>();
  colors = std::vector<glm::vec4>();
  float max_px = 0.98;
  for(std::string line; std::getline(file, line);) {
    std::istringstream sstream(line);
    std::vector<float> numbers;
    for(std::string word; std::getline(sstream, word, ',');)
      numbers.push_back(std::stof(word));

    points.push_back(glm::vec3(numbers[0], -numbers[2], numbers[1]));
    if(color_mode && numbers.size() > 3)
      colors.push_back(glm::vec4(numbers[5], numbers[4], numbers[3], 255.0f) / 255.0f);
    else
      colors.push_back(glm::vec4(0,1.0,0.0,1.0));

    if(colors.back().x > max_px || colors.back().y > max_px || colors.back().z > max_px)
      colors.back() = glm::vec4(0.0);
  }
}

void read_cameras_data(std::string cameras_path,
                       std::vector<glm::vec3>& cameras, std::vector<glm::vec4>& colors) {
  cameras = std::vector<glm::vec3>();
  std::ifstream file(cameras_path);
  std::string header;
  std::getline(file, header);
  cameras = std::vector<glm::vec3>();
  colors = std::vector<glm::vec4>();
  for(std::string line; std::getline(file, line);) {
    std::istringstream sstream(line);
    std::vector<float> numbers;
    for(std::string word; std::getline(sstream, word, ',');)
      numbers.push_back(std::stof(word));

    cameras.push_back(glm::vec3(numbers[0], -numbers[2], numbers[1]));
    colors.push_back(glm::vec4(1.0,0,0,1.0));
  }
}

int main(int argc, char* args[]) {
  auto canvas = std::make_shared<graphics::Canvas>();

  //attach a 3D scene to the canvas
  auto scene_renderer = std::make_shared<graphics::SceneRenderer>();
  auto scene = std::make_shared<graphics::Scene>(canvas);

  //create a camera
  auto camera = std::make_shared<graphics::Camera>();
  scene->camera = camera;
  camera->z_far = 100;
  camera->position = glm::vec3(20,60,-20);
  camera->target = glm::vec3(10, 4, camera->position.z+0.1);
  auto light = std::make_shared<graphics::Light>(graphics::LightType::directional);
  light->type = graphics::LightType::directional;
  scene->lights.push_back(light);

  canvas->clear();

  //draw points
  //crate shader
  graphics::Shader shader(graphics::SHADERS_DIR + "simple3d_vshader.glsl",
                          graphics::SHADERS_DIR + "simple3d_fshader.glsl");
  shader.add_uniform("_proj_mat");
  shader.add_uniform("_view_mat");
  shader.add_uniform("_model_mat");
  shader.add_attribute("_pos");
  shader.add_attribute("_color");
  //read points file
  std::vector<glm::vec3> points_vertices;
  std::vector<glm::vec4> points_colors;
  ::read_points_data("/Users/terna/Downloads/vo_map/points.vio", points_vertices, points_colors);
  auto points = std::make_shared<graphics::PointCloud>(points_vertices, points_colors);
  //set shader stuff
  shader.use();
  shader.set_uniform("_proj_mat", camera->get_proj_mat());
  shader.set_uniform("_view_mat", camera->get_view_mat());
  glm::mat4 model_mat(1.0);// = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1,0,0));
  shader.set_uniform("_model_mat", model_mat);
  //draw
  glPointSize(5);
  points->draw();
  glPointSize(1);

  //draw cameras
  //read cameras file
  std::vector<glm::vec3> cameras_vertices;
  std::vector<glm::vec4> cameras_colors;
  ::read_cameras_data("/Users/terna/Downloads/vo_map/keyframes.vio", cameras_vertices, cameras_colors);
  //create 3D object for the camera and add to the scene
  auto node_geometry = std::make_shared<graphics::Geometry>(graphics::ShapeType::box);
  for(auto cam_pos: cameras_vertices) {
    std::shared_ptr<graphics::Object3D> cam_node = std::make_shared<graphics::Object3D>(node_geometry);
    cam_node->scale = glm::vec3(0.2);
    cam_node->material.color = glm::vec3(0.95,0.1,0.1);
    cam_node->geometry->wire_frame = true;
    cam_node->position = cam_pos;
    scene->add_node(cam_node);
  }
  //draw
  scene_renderer->render_scene(scene);

  canvas->swap_buffers();

  while(canvas->still_open()) {
    canvas->poll_events();
  }
  canvas->close_window();
}
