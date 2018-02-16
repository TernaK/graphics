#include <graphics/scene.h>
#include "rubiks.h"
#include <numeric>
using namespace std;

RubiksCube::RubiksCube() {
  vector<uchar> idxs(27,0);
  std::iota(idxs.begin(), idxs.end(), 0);
  grid = cv::Mat(3,3,CV_8UC3, idxs.data());
}

RubiksCube::Plane RubiksCube::rotate_plane(RubiksCube::Plane plane, bool ckw) {
  cv::Mat face = cv::Mat(3,3,CV_8U, plane.data());
  cv::rotate(face, face, ckw ? cv::ROTATE_90_CLOCKWISE : cv::ROTATE_90_COUNTERCLOCKWISE);
  return plane;
}

void RubiksCube::update_cube(RubiksCube::Plane plane, RubiksCube::Plane new_plane) {
  cv::Mat grid_clone = grid.clone();
  for(int i = 0; i < plane.size(); i++) {
    grid_clone.at<uchar>(plane[i]) = grid.at<uchar>(new_plane[i]);
  }
  grid = grid_clone;
}

int main(int argc, char* args[]) {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>();
  graphics::Scene3D scene(canvas);

  auto cube = RubiksCube();
  auto plane = cube.plane_F;
  auto rotated_plane = cube.rotate_plane(plane);
  cv::Mat face1 = cv::Mat(3,3,CV_8U, plane.data()).clone();
  cv::Mat face2 = cv::Mat(3,3,CV_8U, rotated_plane.data()).clone();
  cout << face1 << endl << face2 << endl;

//  std::shared_ptr<graphics::Solid> earth = graphics::SolidShape::make_sphere(2, 10, 10);
//  earth->material.color = glm::vec3(0.1, 0.1, 0.8);
//
//  scene.add_drawable(earth);
//
//  while(canvas->still_open()) {
//    earth->rotation.y += 1;
//    earth->rotation.x += 1;
//    scene.draw_scene();
//  }
}

