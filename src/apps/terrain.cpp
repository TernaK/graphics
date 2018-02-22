#include <graphics/scene.h>
#include <graphics/solid.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

cv::Mat perlin = cv::Mat(5,5, CV_32F);

float blerp(float p00, float p10, float p01, float p11,
            float x, float y) {
//  x = 3*x*x - 2*x*x*x;
//  y = 3*y*y - 2*y*y*y;
  float y0 = (1.0-x) * p00 + x * p10;
  float y1 = (1.0-x) * p01 + x * p11;
  return (1.0-y) * y0 + y * y1;
}

cv::Mat make_noise(cv::Size size) {
  cv::Mat result = cv::Mat(size, CV_32F);
  result.forEach<float>([&size](float& val, const int* loc) {
    cv::Point2f p(float(loc[0])/size.width * (perlin.cols - 1),
                 float(loc[1])/size.height * (perlin.rows - 1));
    cv::Point l00(floor(p.x), floor(p.y));
    cv::Point l10 = l00 + cv::Point(1,0);
    cv::Point l01 = l00 + cv::Point(0,1);
    cv::Point l11 = l00 + cv::Point(1,1);
    float p00 = perlin.at<float>(l00);
    float p10 = perlin.at<float>(l10);
    float p01 = perlin.at<float>(l01);
    float p11 = perlin.at<float>(l11);
    float x = fmod(p.x, 1.0);
    float y = fmod(p.y, 1.0);
    val = blerp(p00, p10, p01, p11, x, y);
  });
  return result;
}

int main(int argc, char* args[]) {
  perlin.forEach<float>([](float& val, const int* pos) {
    val = (float(rand() % 512) / 256.0) - 1.0;
  });

  cv::imshow("noise", make_noise(cv::Size(500,500)));
  cv::waitKey();
//  shared_ptr<Canvas> canvas = make_shared<Canvas>();
//  Scene3D scene(canvas);
//  scene.camera->position = glm::vec3(5,10,5);
//  ((PointLight*)scene.light.get())->position = glm::vec3(5,5,0);
//
//  Geometry terrain_geometry = Geometry::create_terrain(20, 20);
//  shared_ptr<Solid> terrain = make_shared<Solid>(terrain_geometry);
//  terrain->scale = glm::vec3(5);
//
//  scene.add_drawable(terrain);
//
//  while(canvas->still_open()) {
//    scene.clear();
//    terrain->rotation.y += 1;
//
//    canvas->poll_events();
//    scene.draw_scene();
//  }
}
