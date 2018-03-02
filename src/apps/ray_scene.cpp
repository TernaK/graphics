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
  node.scale *= 1.5;

  Light light;
  light.position = glm::vec3(10,10,10);

//  Camera

  Transformable parent_transformable;

//  for(;;) {
    parent_transformable.position.y = 0.4 * sin(glfwGetTime());
    transform_t p_transform = parent_transformable.get_transform();
    cv::Mat image = cv::Mat::zeros(400,400,CV_32FC3);
    node.position.x = 1 * sin(glfwGetTime());
    node.position.y = 1 * cos(glfwGetTime());
    node.rotation.z += 3;
    node.rotation.y += 5;
    node.rotation.x += 3;
    transform_t transform = node.get_transform() << p_transform;
    glm::vec3 clear_color(0.1);

    int width = image.cols;
    int height = image.rows;
//    for(int i = 0; i < image.rows; i++) {
//      for(int j = 0; j < image.cols; j++) {
    image.forEach<cv::Vec3f>([&](cv::Vec3f& frag, const int* row_col) {
      //make ray
        ray_t ray;
        ray.p = glm::vec3(0,0,10);
        int row = row_col[0];
        int col = row_col[1];
//        auto& frag = image.at<cv::Vec3f>(i,j);
        float ray_dx = (float(col) * 2.0 / width) - 1.0;
        float ray_dy = -((float(row) * 2.0 / height) - 1);
        ray.d = glm::normalize(glm::vec3(ray_dx, ray_dy, -3));

        hit_t hit;
        float cos_t = 0;
        Material material;
        glm::vec3 color = clear_color;
        if(node.ray_hit_test(ray, hit, transform, material)) {
          material.shininess = 128;
          material.strength.x = 0.3;
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
        frag = cv::Vec3f(color.b, color.g, color.r);
//      }
//    }
    });

    cv::imshow("ray cast", image);
    cv::waitKey();
//  }

}
