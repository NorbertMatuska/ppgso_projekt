#include "plane.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Plane::mesh;
std::unique_ptr<ppgso::Shader> Plane::shader;

Plane::Plane() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
}

bool Plane::update(float dTime, Scene &scene) {
    return true;
}

void Plane::render(const Camera& camera) {
    shader->use();
    shader->setUniform("OverallColor", glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(30.0f));
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    mesh->render();
}