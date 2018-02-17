#include <graphics/shape.h>
#include <graphics/canvas.h>
#include <graphics/sprite.h>
#include <graphics/light.h>
#include <graphics/camera.h>
#include <graphics/framebuffer.h>
using namespace std;
using namespace graphics;

int main() {
  shared_ptr<graphics::Canvas> canvas = make_shared<graphics::Canvas>(800,600,false);
  int width, height;
  canvas->get_true_frame_size(width, height);
  
  auto post_shader = make_shared<Shader>(std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_vshader.glsl",
                                         std::string(GRAPHICS_SHADERS_DIRECTORY) + "sprite_fshader_post.glsl");
  post_shader->use();
  post_shader->add_attribute("_pos");
  post_shader->add_attribute("_tex_coord");
  post_shader->add_uniform("_texture0");
  post_shader->add_uniform("_time");
  post_shader->add_uniform("_width");
  post_shader->add_uniform("_height");
  post_shader->set_uniform("_width", float(width));
  post_shader->set_uniform("_height", float(height));

  auto solid_shader = Shader::make_solid_point_shader();

  PointLight light;
  Camera camera;
  camera.position.z = 7;
  camera.aspect_ratio = canvas->get_aspect_ratio();

  graphics::Framebuffer framebuffer(canvas, true, false);
  shared_ptr<graphics::Sprite> sprite = std::make_shared<graphics::Sprite>(framebuffer.color_texture);
  shared_ptr<graphics::Solid> solid = graphics::SolidShape::make_sphere(1,2,4);
  shared_ptr<graphics::Solid> cube = graphics::SolidShape::make_cube();
  cube->material.color = glm::vec4(1,0,0,1);

  while(canvas->still_open()) {
    framebuffer.begin_render(); //first bind framebuffer
    //render the scene
    canvas->clear();
    solid->position.x = 3 * cos(2*M_PI*glfwGetTime() * 0.2);
    solid->position.z = -3 * sin(2*M_PI*glfwGetTime() * 0.2);
    solid->rotation.y += 1;
    cube->rotation.y -= 1.5;
    cube->rotation.z += 1;

    solid_shader->use();
    light.set_uniforms(solid_shader);
    camera.set_uniforms(solid_shader);
    solid->draw(solid_shader);
    cube->draw(solid_shader);
    framebuffer.end_render(); //unbind framebuffer

    //render the post-shaded result
    canvas->clear();
    post_shader->use();
    post_shader->set_uniform("_time", (float)glfwGetTime());
    sprite->draw(post_shader);

    canvas->poll_events(); //don't forget to poll for events otherwise nothing will show
    canvas->swap_buffers(); //and of course swap buffers
  }
}
