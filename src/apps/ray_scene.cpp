#include <graphics/scene.h>
#include <graphics/object3d.h>
#include <graphics/scene_renderer.h>
#include <graphics/implicit.h>
#include <graphics/ray.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace graphics;

int main(int argc, char* args[]) {
  cv::Size frame_size(640, 480);
  int supersample_factor = 5;
  shared_ptr<Canvas> canvas = make_shared<Canvas>(frame_size.width,frame_size.height,true);

  ImplicitNode node = ImplicitNode(ShapeType::sphere);
  node.scale = glm::vec3(2);

  Light light;
  light.position = glm::vec3(6,6,6);

//  Camera
  Camera camera;
  camera.position = glm::vec3(0,0,6);
  camera.aspect_ratio = canvas->get_aspect_ratio();

//  glm::vec3 cam_d = glm::normalize(camera.target - camera.position);
  float y_near = camera.z_near * tan(glm::radians(camera.fovy/2.0));
  float x_near = camera.aspect_ratio * y_near;

  Transformable parent_transformable;

//  for(;;) {
    parent_transformable.position.y = 0.4 * sin(glfwGetTime());
    transform_t p_transform = parent_transformable.get_transform();
//  cv::Size size(canvas->width, canvas->height);
    cv::Mat image = cv::Mat::zeros(frame_size * supersample_factor, CV_32FC3);
//    node.position.x = 2 * sin(glfwGetTime());
//    node.position.y = 2 * cos(glfwGetTime());
    node.rotation.z += 3;
    node.rotation.y += 5;
//    node.rotation.x += 3;
    transform_t transform = node.get_transform() << p_transform;
    glm::vec3 clear_color(0.1);

    int width = image.cols;
    int height = image.rows;

    image.forEach<cv::Vec3f>([&](cv::Vec3f& frag, const int* row_col) {
      int row = row_col[0];
      int col = row_col[1];
      //TODO: find the correct vectors
      float ray_dx = ((2.0 * col - width) / width) * x_near;
      float ray_dy = -((2.0 * row - height) / height) * y_near;

      //make ray
      ray_t ray;
      ray.p = glm::vec3(0,0,10);
      ray.d = glm::normalize(glm::vec3(ray_dx, ray_dy, -camera.z_near));

      hit_t hit;
      float cos_t = 0;
      Material material;
      glm::vec3 color = clear_color;
      if(node.hit_test(ray, hit, transform, material)) {
        material.shininess = 32;
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
    });

    cv::GaussianBlur(image, image, cv::Size(3,3), 1.2);
    cv::resize(image, image, frame_size);
    cv::imshow("ray cast", image);
    cv::waitKey();
//  }

}
