#include "building.h"
#include "camera.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Shader> Building::shader;

Building::Building(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : position(initialPosition) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);

    // Load the mesh
    mesh = std::make_unique<ppgso::Mesh>(objFilename);

    // Load the texture
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));
}

bool Building::update(float dTime, Scene &scene) {
    return true;
}

void Building::render(const Camera& camera) {
    shader->use();
    shader->setUniform("OverallColor", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setUniform("Texture", *texture);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position); // Use position from constructor
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f)); // Adjust scale if needed

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    mesh->render();
}
