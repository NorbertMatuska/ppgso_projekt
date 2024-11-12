#include "building.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Building::mesh;
std::unique_ptr<ppgso::Texture> Building::texture;
std::unique_ptr<ppgso::Shader> Building::shader;

Building::Building() {

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("corsair.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("small_house.obj");
}