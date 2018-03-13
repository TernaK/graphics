#pragma once
#include <graphics/buffer_object.h>
#include <graphics/scene_node.h>
#include <graphics/material.h>
#include <graphics/texture.h>
#include <numeric>

namespace graphics {
  struct Object3D : public SceneNode {
    Material material;
    
    Object3D() = default;

    Object3D(std::shared_ptr<Geometry> geometry, Material material = Material());

    ~Object3D();

    void set_uniforms(Transform transform) override;
  };

//  struct Object3DTex : public SceneNode {
//    std::shared_ptr<Texture> texture;
//
//    Object3DTex() = default;
//
//    Object3DTex(std::shared_ptr<Geometry> geometry,
//                std::shared_ptr<Texture> texture);
//
//    ~Object3DTex();
//
//    void set_uniforms(Transform transform) override;
//  };
}
