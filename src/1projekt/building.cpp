#include "building.h"
#include "camera.h"
#include "shaders/texture_vert_glsl.h"
#include "shaders/texture_frag_glsl.h"
#include <glm/gtc/matrix_transform.hpp>

// Static resources
std::unique_ptr<ppgso::Shader> Building::shader;
// Initialize the ambient light color (warm moonlight)
glm::vec3 Building::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f); // Adjust as needed for warmth

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

ppgso::Shader* Building::getShader() const {
    return shader.get();
}

void Building::render(const Camera& camera) {
    shader->use();
    shader->setUniform("Texture", *texture);

    // Set the ambient light color uniform
    shader->setUniform("AmbientLightColor", ambientLightColor);

    // Build the model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    // Set transformation uniforms
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    shader->setUniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));  // Max ambient reflectivity
    shader->setUniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));  // Max diffuse reflectivity
    shader->setUniform("material.specular", glm::vec3(1.0f, 1.0f, 1.0f)); // Max specular reflectivity
    shader->setUniform("material.shininess", 64.0f);

    // Render the mesh
    mesh->render();
}
