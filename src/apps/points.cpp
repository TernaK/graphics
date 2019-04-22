#include "graphics/point_cloud.h"
#include "graphics/object3d.h"
#include "graphics/scene_renderer.h"
#include <string>
#include <glm/gtx/functions.hpp>

const glm::mat4 FROM_CV_TRANSFORM = glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1,0,0));

/* NOTE: opengl x => opencv x, opengl y => opencv -z, opengl z => opencv y */
glm::vec3 transform_from_opencv(const glm::vec3& vec) {
  return glm::vec3(FROM_CV_TRANSFORM * glm::vec4(vec, 1.0));
}

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
    points.push_back(glm::vec3(numbers[0], numbers[1], numbers[2]));
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

    cameras.push_back(glm::vec3(numbers[0], numbers[1], numbers[2]));
    colors.push_back(glm::vec4(1.0,0,0,1.0));
  }
}

int main(int argc, char* args[]) {
  //read points file
  std::vector<glm::vec3> points_vertices;
  std::vector<glm::vec4> points_colors;
  ::read_points_data("/Users/terna/Downloads/vo_map/points.vio", points_vertices, points_colors);
  //read cameras file
  std::vector<glm::vec3> cameras_vertices;
  std::vector<glm::vec4> cameras_colors;
  ::read_cameras_data("/Users/terna/Downloads/vo_map/keyframes.vio", cameras_vertices, cameras_colors);

  auto canvas = std::make_shared<graphics::Canvas>();

  //attach a 3D scene to the canvas
  auto scene_renderer = std::make_shared<graphics::SceneRenderer>();
  auto scene = std::make_shared<graphics::Scene>(canvas);

  //create a camera & light
  auto camera = std::make_shared<graphics::Camera>();
  camera->z_far = 300;
  scene->camera = camera;
  auto light = std::make_shared<graphics::Light>(graphics::LightType::directional);
  light->type = graphics::LightType::directional;
  scene->lights.push_back(light);

  //create point cloud
  auto point_cloud = std::make_shared<graphics::PointCloud>(points_vertices, points_colors, camera);
  point_cloud->rotation.x = 90; //frmo opengl space

  //create 3D object for the camera and add to the scene
  auto node_geometry = std::make_shared<graphics::Geometry>(graphics::ShapeType::box);
  for(auto cam_pos: cameras_vertices) {
    std::shared_ptr<graphics::Object3D> cam_node = std::make_shared<graphics::Object3D>(node_geometry);
    cam_node->scale = glm::vec3(0.2);
    cam_node->material.color = glm::vec3(0.95,0.1,0.1);
    cam_node->geometry->wire_frame = true;
    cam_node->position = ::transform_from_opencv(cam_pos);
    scene->add_node(cam_node);
  }

  const auto &stats = point_cloud->get_stats();
  //target the center of the distribution
  glm::vec3 range = glm::abs(stats.max_pt - stats.min_pt);
  glm::vec3 corner;
  corner.x = stats.mean_pt.x + 1.5 * range.x/2.0f;
  corner.y = stats.mean_pt.y - range.y/2.0f;
  corner.z = -20;//stats.mean_pt.z - 1.5 * range.z;
  corner = glm::normalize(corner);
  float radius = glm::length(range);

  auto start_pos = ::transform_from_opencv(corner*radius*3.0f);
  auto start_target = ::transform_from_opencv(stats.mean_pt);
  auto start_vec = start_pos - start_target;
  camera->target = ::transform_from_opencv(stats.mean_pt);


  cv::TickMeter tm;
  tm.start();

  while(canvas->still_open()) {
    tm.stop();
    float time = tm.getTimeMilli() * 1E-3f;
    tm.start();
    float rot_rad = fmod(time, 10.0f) / 10.0f * 2.0f*M_PI;
    std::cerr << rot_rad << std::endl;
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), rot_rad, glm::vec3(0,1,0));
    camera->position = start_target + glm::vec3(rot * glm::vec4(start_vec, 1.0));

    //draw
    canvas->clear();
    //draw the points
    point_cloud->draw();
    //draw the rest of the scene
    scene_renderer->render_scene(scene);
    canvas->poll_events();
    canvas->swap_buffers();
  }
  canvas->close_window();
}
