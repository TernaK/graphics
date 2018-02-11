#pragma once
#include <graphics/texture.h>
#include <memory>

namespace graphics {
  class Sprite {
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
    void draw() const;
  };
}
