#pragma once
#include <graphics/texture.h>

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
    
    void init();

    void bind_vertex_data();
    
    void release_vertex_data();
    
    void draw(std::shared_ptr<Shader> shader,
              bool draw_children = true,
              const glm::mat4& p_model = glm::mat4(1.0),
              const glm::mat3& p_model_n = glm::mat4(1.0)) const;
  };
}
