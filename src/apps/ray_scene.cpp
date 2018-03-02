#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <graphics/implicit.h>
#include <graphics/ray.h>
#include <opencv2/opencv.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>(400,300,true);

  ImplicitNode node = ImplicitNode(ShapeType::sphere);
  //make ray
  ray_t ray;
  ray.p = glm::vec3(0,0,6);

  glm::vec3 light_position = glm::vec3(10,10,10);

  Transformable parent_transformable;

  for(;;) {
    parent_transformable.position.y = 0.4 * sin(glfwGetTime());
    transform_t p_transform = parent_transformable.get_transform();
    cv::Mat image = cv::Mat::zeros(200,200,CV_32FC3);
    node.position.x = sin(glfwGetTime());
    node.position.y = cos(glfwGetTime());
//    node.rotation.z += 3;
//    node.rotation.y += 5;
//    node.rotation.x += 3;
//    p_transform.mo
    transform_t transform = node.get_transform() << p_transform;

    for(int i = 0; i < image.rows; i++) {
      for(int j = 0; j < image.cols; j++) {
        ray.d = glm::normalize(glm::vec3((float(j) * 2.0 / image.cols) - 1.0, -((float(i) * 2.0 / image.rows) - 1), -3));

        hit_t hit;
        float cos_t = 0;
        cv::Vec3f color(0.2,0.2,0.2);
        if(node.ray_hit_test(ray, hit, transform)) {
          cos_t = glm::dot(glm::normalize(light_position - hit.p), hit.n);
          cos_t = cos_t < 0 ? 0 : cos_t;
          color = cv::Vec3f(0.1,0.7,0.7) * cos_t;
        }

        image.at<cv::Vec3f>(i,j) = color;
      }
    }

    cv::imshow("ray cast", image);
    cv::waitKey(30);
  }

}
