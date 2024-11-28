#include "plane.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Plane::mesh;
std::unique_ptr<ppgso::Shader> Plane::shader;
std::unique_ptr<ppgso::Texture> Plane::texture;

// Constructor with position parameter
Plane::Plane(const glm::vec3& position) : position(position) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/asphalt.bmp"));
}

bool Plane::update(float dTime, Scene &scene) {
    return true;
}

void Plane::render(const Camera& camera) {
    shader->use();
    shader->setUniform("Texture", *texture);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Apply transformations: position, rotation, and scale
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    mesh->render();
}
