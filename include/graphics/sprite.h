#pragma once
#include <graphics/drawable.h>
#include <graphics/texture.h>

namespace graphics {
  class Sprite : public Drawable {
  private:
    GLuint vbo;
    GLuint vao;
    void create_data();

  public:
    std::vector<GLfloat> vertex_data;
    std::shared_ptr<Texture> texture;
    
    Sprite(std::string image);
    Sprite(cv::Mat image);
    Sprite(std::shared_ptr<Texture> texture);

    void bind_vertex_data();
    void release_vertex_data();
    ShaderType get_shader_type() const override;
    void draw(std::shared_ptr<Shader> shader) const override;
  };
}
