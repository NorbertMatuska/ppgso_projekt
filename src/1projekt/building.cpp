#include "building.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Shader> Building::shader;
glm::vec3 Building::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);

// Initialize static members from Renderable
glm::mat4 Renderable::lightSpaceMatrix;
GLuint Renderable::depthMap;

Building::Building(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : position(initialPosition) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);

    // Load the mesh
    mesh = std::make_unique<ppgso::Mesh>(objFilename);

    // Load the texture
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));
}

bool Building::update(float dTime, Scene& scene) {
    // No dynamic behavior for buildings in this example
    return true;
}

void Building::render(const Camera& camera) {
    shader->use();

    // Set transformation matrices
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    shader->setUniform("LightSpaceMatrix", lightSpaceMatrix);

    // Set material properties
    shader->setUniform("material.ambient", glm::vec3(1.0f));
    shader->setUniform("material.diffuse", glm::vec3(1.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f));
    shader->setUniform("material.shininess", 64.0f);

    // Set the texture
    shader->setUniform("Texture", *texture);

    // Bind the shadow map
    shader->setUniform("ShadowMap", 1); // Texture unit 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // Render the mesh
    mesh->render();
}

void Building::renderDepth(ppgso::Shader& depthShader) {
    depthShader.use();

    // Set transformation matrices
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    depthShader.setUniform("ModelMatrix", modelMatrix);

    // Render the mesh
    mesh->render();
}

ppgso::Shader* Building::getShader() const {
    return shader.get();
}
