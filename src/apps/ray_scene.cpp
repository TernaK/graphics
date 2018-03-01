#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <graphics/ray.h>
#include <opencv2/opencv.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  shared_ptr<Canvas> canvas = make_shared<Canvas>(400,300,true);

//  auto tree = Octree<int>(2);
//  cout << tree.get_level();

  auto mesh = make_shared<Mesh>(MeshType::box);
  Transformable transformable;
//  transformable.rotation = glm::vec3(90,0,0);
//  transform_t transform = transformable.get_transform();

  //make ray
  ray_t ray;
  ray.p = glm::vec3(0,0,5);

  glm::vec3 light_position = glm::vec3(10,10,10);

  for(;;) {
    cv::Mat image = cv::Mat::zeros(200,200,CV_32FC3);
//    transformable.position.x = 2 * sin(glfwGetTime());
//    transformable.position.y = 2 * cos(glfwGetTime());
//    transformable.rotation.z += 3;
    transformable.rotation.y += 5;
//    transformable.rotation.x = 90;
    transform_t transform = transformable.get_transform();
    for(int i = 0; i < image.rows; i++) {
      for(int j = 0; j < image.cols; j++) {
        ray.d = glm::normalize(glm::vec3((float(j) * 2.0 / image.cols) - 1.0, -((float(i) * 2.0 / image.rows) - 1), -2));
//        ray.d = glm::vec3(0,0,-1);

        cv::Vec3f color(0.2,0.2,0.2);

        hit_t hit;
        if(mesh->ray_hit_test(ray, hit, transform)) {
          float cos_t = glm::dot(glm::normalize(light_position - hit.p), hit.n);
          if(cos_t < 0) cos_t = 0.1;
          color = cv::Vec3f(0.1,0.7,0.7) * cos_t;
        }

        image.at<cv::Vec3f>(i,j) = color;
      }
    }

    cv::imshow("ray cast", image);
    cv::waitKey(30);
  }

}
