#include <graphics/sprite.h>

using namespace std;
using namespace graphics;

Sprite::Sprite(std::string image)
: texture(make_shared<Texture>(image)) {
//  init_shader_type();
  create_data();
  bind_vertex_data();
}

Sprite::Sprite(cv::Mat image)
: texture(make_shared<Texture>(image)) {
//  init_shader_type();
  create_data();
  bind_vertex_data();
}

Sprite::Sprite(std::shared_ptr<Texture> texture)
: texture(texture) {
//  init_shader_type();
  create_data();
  bind_vertex_data();
}

void Sprite::create_data() {
  if(!texture->is_filpped()) {
    vertex_data = {
      -1,1,0, 0,0,  1,1,0, 1,0,   -1,-1,0, 0,1,
      1,1,0, 1,0,   1,-1,0, 1,1,  -1,-1,0, 0,1
    };
  } else {
    vertex_data = {
      -1,1,0, 0,1,  1,1,0, 1,1,   -1,-1,0, 0,0,
      1,1,0, 1,1,   1,-1,0, 1,0,  -1,-1,0, 0,0
    };
  }
}

void Sprite::bind_vertex_data() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat), vertex_data.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Sprite::release_vertex_data() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Sprite::draw(std::shared_ptr<Shader> shader,
                  const glm::mat4& p_model,
                  const glm::mat3& p_model_n,
                  bool draw_children) const {
  texture->activate();
  texture->set_uniforms(shader);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

ShaderType Sprite::get_shader_type() const {
  return ShaderType::Sprite;
}
