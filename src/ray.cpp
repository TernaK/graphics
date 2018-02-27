#include <graphics/ray.h>
using namespace std;
using namespace graphics;

PrimitiveSceneNode::PrimitiveSceneNode(std::shared_ptr<PrimitiveGeometry> geometry)
: SceneNode(geometry) {

}

RaySceneRenderer::RaySceneRenderer() : SceneRenderer() {

}

void RaySceneRenderer::render_scene(std::shared_ptr<Scene> scene) {

}
