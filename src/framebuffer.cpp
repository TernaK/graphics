#include <graphics/framebuffer.h>

using namespace std;
using namespace graphics;

Framebuffer::Framebuffer(std::shared_ptr<Canvas> _canvas,
                         bool _use_color,
                         bool _use_depth)
: use_color(_use_color), use_depth(_use_depth), canvas(_canvas) {
  if(use_color && use_depth == 0)
    throw runtime_error("Framebuffer must use the color or(and) depth buffers");

  create_textures();
  create_framebuffer();
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::create_textures() {
  int w, h;
  glfwGetFramebufferSize(canvas->window, &w, &h);
  if(use_color) {
    GLuint color_tex_id;
    glGenTextures(1, &color_tex_id);
    glBindTexture(GL_TEXTURE_2D, color_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    color_texture = make_shared<Texture>(color_tex_id);
    color_texture->set_filpped();
  }
  if(use_depth) {
    GLuint depth_tex_id;
    glGenTextures(1, &depth_tex_id);
    glBindTexture(GL_TEXTURE_2D, depth_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    depth_texture = make_shared<Texture>(depth_tex_id);
    depth_texture->set_filpped();
  }
}

void Framebuffer::create_framebuffer() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->get_texture_id(), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->get_texture_id(), 0);

  //check completeness
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    throw runtime_error("framebuffer is incomplete");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::begin_render() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::end_render() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

cv::Mat Framebuffer::read_to_mat() {
  int width, height;
  canvas->get_true_frame_size(width, height);
  cv::Mat image(height, width, CV_8UC3);
  ///https://stackoverflow.com/questions/9097756/converting-data-from-glreadpixels-to-opencvmat/9098883
  //use fast 4-byte alignment (default anyway) if possible
  glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);
  //set length of one complete row in destination data (doesn't need to equal img.cols)
  glPixelStorei(GL_PACK_ROW_LENGTH, image.step/image.elemSize());
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glReadPixels(0, 0, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);
  cv::flip(image, image, 0);
  return image;
}
