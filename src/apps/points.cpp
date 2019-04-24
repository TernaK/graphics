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

glm::vec3 transform_from_opengl(const glm::vec3& vec) {
  return glm::vec3(glm::transpose(FROM_CV_TRANSFORM) * glm::vec4(vec, 1.0));
}

void read_points_data(std::string points_path,
                      std::vector<glm::vec3>& points, std::vector<glm::vec4>& colors,
                      bool color_mode = true) {
  glm::vec4 default_color = glm::vec4(0,1.0,0.0,1.0);
  std::ifstream file(points_path);
  std::string header;
  std::getline(file, header);
  points = std::vector<glm::vec3>();
  colors = std::vector<glm::vec4>();
  float max_px = 0.99;
  for(std::string line; std::getline(file, line);) {
    std::istringstream sstream(line);
    std::vector<float> numbers;
    for(std::string word; std::getline(sstream, word, ',');)
      numbers.push_back(std::stof(word));
    points.push_back(glm::vec3(numbers[0], numbers[1], numbers[2]));
    if(color_mode && numbers.size() > 3)
      colors.push_back(glm::vec4(numbers[5], numbers[4], numbers[3], 255.0f) / 255.0f);
    else
      colors.push_back(default_color);

    if(colors.back().x > max_px || colors.back().y > max_px || colors.back().z > max_px)
      colors.back() = glm::vec4(0);
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

float SPEED = 0.06;
float ZOOM_SPEED = 0.02;
std::shared_ptr<graphics::Camera> camera;
glm::vec3 pitch_yaw_zoom = {0,0,1};
void key_callback(GLFWwindow* window, int key, int code, int action, int) {
  float new_zoom = 1;
  switch (key) {
    case GLFW_KEY_UP:
      pitch_yaw_zoom[0] -= SPEED;
      std::cerr << "pitch -: " << pitch_yaw_zoom[0] << std::endl;
      break;
    case GLFW_KEY_DOWN:
      pitch_yaw_zoom[0] += SPEED;
      std::cerr << "pitch +: " << pitch_yaw_zoom[0] << std::endl;
      break;
    case GLFW_KEY_LEFT:
      pitch_yaw_zoom[1] -= SPEED;
      std::cerr << "yaw -: " << pitch_yaw_zoom[1] << std::endl;
      break;
    case GLFW_KEY_RIGHT:
      pitch_yaw_zoom[1] += SPEED;
      std::cerr << "yaw +: " << pitch_yaw_zoom[1] << std::endl;
      break;
    case GLFW_KEY_I:
      new_zoom = std::max(0.02f, fabs(pitch_yaw_zoom[2] - ZOOM_SPEED));
      pitch_yaw_zoom[2] = new_zoom;
      std::cerr << "zoom +: " << pitch_yaw_zoom[2] << std::endl;
      break;
    case GLFW_KEY_O:
      new_zoom = std::max(0.02f, fabs(pitch_yaw_zoom[2] + ZOOM_SPEED));
      pitch_yaw_zoom[2] = new_zoom;
      std::cerr << "zoom -: "<< pitch_yaw_zoom[2]  << std::endl;
      break;
    default: break;
  }
}

glm::vec3 get_camera_vector() {
  glm::vec3 vec = glm::vec3(cos(pitch_yaw_zoom[0]) * sin(pitch_yaw_zoom[1]),
                            sin(pitch_yaw_zoom[0]),
                            cos(pitch_yaw_zoom[0]) * cos(pitch_yaw_zoom[1]));
  return vec;
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
  //attach key callback
  glfwSetKeyCallback(canvas->window, key_callback);

  //attach a 3D scene to the canvas
  auto scene_renderer = std::make_shared<graphics::SceneRenderer>();
  auto scene = std::make_shared<graphics::Scene>(canvas);

  //create a camera & light
  camera = std::make_shared<graphics::Camera>();
  camera->z_far = 300;
  camera->aspect_ratio = canvas->get_aspect_ratio();
  scene->camera = camera;
  auto light = std::make_shared<graphics::Light>(graphics::LightType::directional);
  light->type = graphics::LightType::directional;
  light->direction = glm::vec3(glm::transpose(FROM_CV_TRANSFORM) * glm::vec4(light->direction, 1.0));
  scene->lights.push_back(light);

  //create point cloud
  auto point_cloud = std::make_shared<graphics::PointCloud>(points_vertices, points_colors, camera);
  point_cloud->rotation.x = 90; //from opengl space

  //create 3D object for the camera and add to the scene
  graphics::PrimitiveParams params = {7,7};
  auto node_geometry = std::make_shared<graphics::Geometry>(graphics::ShapeType::sphere, params);
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
  float radius = glm::length(range) * 1.2;
  camera->target = ::transform_from_opencv(stats.mean_pt);

  cv::TickMeter tm;
  tm.start();

  scene->clear_color = glm::vec4(0);

  bool init = true;
  while(canvas->still_open()) {
    camera->position = camera->target + radius * get_camera_vector() * pitch_yaw_zoom[2];

    //draw
    scene->clear();
    point_cloud->draw();
    scene_renderer->render_scene(scene);
    graphics::CanvasHelper::check_gl_errors();
    canvas->poll_events();
    canvas->swap_buffers();

    //MOJAVE HACK!!!
    if(init) {
      init = false;
      glfwSetWindowPos(canvas->window, 100, 100);
    }
  }
  canvas->close_window();
}
