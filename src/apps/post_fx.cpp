#include <graphics/framebuffer.h>
#include <graphics/scene.h>
#include <graphics/shape.h>
#include <graphics/sprite.h>
using namespace std;

int main() {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>(800,600);
  graphics::Scene3D scene(canvas);
  scene.clear_color = glm::vec4(0);
  scene.sprite_shader = make_shared<graphics::SpriteShader>(std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_vshader.glsl",
                                                            std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_fshader_post.glsl");
  graphics::Framebuffer framebuffer(canvas, true, true);

  shared_ptr<graphics::Sprite> sprite = std::make_shared<graphics::Sprite>(framebuffer.color_texture);
  shared_ptr<graphics::Solid> solid = graphics::SolidShape::make_sphere(2, 30, 30);
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while(!canvas->still_open()) {
    //render the scene
    scene.drawables.clear();
    framebuffer.begin_render();
    scene.add_drawable(solid);
    solid->position.x = 3 * cos(2*M_PI*glfwGetTime() * 0.2);
    solid->position.z = -3 * sin(2*M_PI*glfwGetTime() * 0.2);
    scene.draw_scene();
//    cv::Mat image = framebuffer.read_to_mat();
    framebuffer.end_render();

//    cv::imshow("image", image);
//    cv::waitKey(30);
//  }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //render the post-shaded result
    scene.drawables.clear();
    scene.add_drawable(sprite);
    scene.draw_scene();
  }
}
