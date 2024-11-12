#include "building.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Building::mesh;
std::unique_ptr<ppgso::Shader> Building::shader;

Building::Building() {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("small_house_1.obj");
}

bool Building::update(float dTime, Scene &scene) {
    return true;
}

void Building::render(const Camera& camera) {
    shader->use();
    shader->setUniform("OverallColor", glm::vec3(0.5f, 0.5f, 0.5f));
    texture->bind();

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // Adjust position as needed
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f)); // Adjust scale if needed

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    mesh->render();

}
