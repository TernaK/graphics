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
//  node.scale *= 2;
  //make ray
  ray_t ray;
  ray.p = glm::vec3(0,0,10);

  Light light;
  light.position = glm::vec3(10,10,10);

//  Camera

  Transformable parent_transformable;

//  for(;;) {
    parent_transformable.position.y = 0.4 * sin(glfwGetTime());
    transform_t p_transform = parent_transformable.get_transform();
  cv::Mat image = cv::Mat::zeros(100,100,CV_32FC3);
    node.position.x = 0.5 * sin(glfwGetTime());
    node.position.y = 0.5 * cos(glfwGetTime());
    node.rotation.z += 3;
    node.rotation.y += 5;
    node.rotation.x += 3;
    transform_t transform = node.get_transform() << p_transform;
    glm::vec3 clear_color(0.1);

//    for(int i = 0; i < image.rows; i++) {
//      for(int j = 0; j < image.cols; j++) {
  image.forEach<cv::Vec3f>([&](cv::Vec3f& frag, const int* loc) {
//    printf("%d %d\n", loc[1], loc[0]);
        ray.d = glm::normalize(glm::vec3((float(loc[1]) * 2.0 / image.cols) - 1.0, -((float(loc[0]) * 2.0 / image.rows) - 1), -3));

        hit_t hit;
        float cos_t = 0;
        Material material;
        glm::vec3 color = clear_color;
        if(node.ray_hit_test(ray, hit, transform, material)) {
          material.shininess = 2;
          material.strength.x = 0.4;
          material.color = glm::vec3(1.0, 0, 0);
          glm::vec3 ambient, diffuse, specular;
          ambient = material.strength.x * light.ambient;
          glm::vec3 l_vec = glm::normalize(light.position - hit.p);
          cos_t = glm::dot(l_vec, hit.n);
          cos_t = cos_t < 0 ? 0 : cos_t;
          diffuse = material.strength.y * light.color * cos_t;
          glm::vec3 r = glm::reflect(-l_vec, hit.n);
          glm::vec3 v = glm::normalize(ray.p - hit.p);
          float spec = glm::dot(r, v);
          spec = spec < 0 ? 0 : spec;
          specular = material.strength.z * light.color * pow(spec, material.shininess);
          color = (ambient + diffuse + specular) * material.color;
          color = glm::clamp(color, 0.0f, 1.0f);
        }
    image.at<cv::Vec3f>(loc[0], loc[1]) = cv::Vec3f(color.b, color.g, color.r);
//      }
//    }
  });

    cv::imshow("ray cast", image);
    cv::waitKey();
//  }

}
