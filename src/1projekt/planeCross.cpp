#include "planeCross.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> PlaneCross::mesh;
std::unique_ptr<ppgso::Shader> PlaneCross::shader;
std::unique_ptr<ppgso::Texture> PlaneCross::texture;
glm::vec3 PlaneCross::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);

PlaneCross::PlaneCross(const glm::vec3& position) : position(position) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("quad.obj");
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/cross.bmp"));
}

bool PlaneCross::update(float dTime, Scene &scene) {
    return true;
}

ppgso::Shader* PlaneCross::getShader() const {
    return shader.get();
}

void PlaneCross::renderDepth(ppgso::Shader& depthShader) {
    depthShader.use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    depthShader.setUniform("ModelMatrix", modelMatrix);

    mesh->render();
}


void PlaneCross::render(const Camera& camera) {
    shader->use();
    shader->setUniform("Texture", *texture);

    shader->setUniform("AmbientLightColor", ambientLightColor);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    shader->setUniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setUniform("material.shininess", 32.0f);

    mesh->render();
}

