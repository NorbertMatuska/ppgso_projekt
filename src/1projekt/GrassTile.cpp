#include "grasstile.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_grass_glsl.h>
#include <shaders/texture_frag_grass_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> GrassTile::mesh;
std::unique_ptr<ppgso::Shader> GrassTile::shader;
std::unique_ptr<ppgso::Texture> GrassTile::texture;

GrassTile::GrassTile(const glm::vec3& position, const glm::vec3& scale) : position(position), scale(scale) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_grass_glsl, texture_frag_grass_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/grass.bmp"));
}

bool GrassTile::update(float dTime, Scene &scene) {
    return true;
}

void GrassTile::render(const Camera& camera) {
    shader->use();
    shader->setUniform("Texture", *texture);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Apply transformations: translation and scale
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    float tilingFactor = 50.0f; // Adjust this to control how often the grass texture tiles
    shader->setUniform("TilingFactor", tilingFactor);

    mesh->render();
}
