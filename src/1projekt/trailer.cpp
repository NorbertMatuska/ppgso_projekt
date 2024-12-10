#include "trailer.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

std::unique_ptr<ppgso::Shader> Trailer::shader;
glm::vec3 Trailer::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);

Trailer::Trailer(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : direction(1.0f, 0.0f, 0.0f), boundingBox(glm::vec3(1.0f)), position(initialPosition) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>(objFilename);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));

}

double normalizeAngle(double angle) {
    angle = fmod(angle, 360);
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

bool Trailer::update(float dTime, Scene& scene) {
    if (auto* car = getParentCar()) {
        position = car->getPosition() + relativePosition;
        position += car->getDirection() * dTime * 5.0f;
        rotation = car->getRotation();

        float height = 1.7f;
        float offsetX = 0.0f;
        float offsetY = 0.0f;

        if (normalizeAngle(rotation) == 0.0f) {
            offsetY = -4.0f;
            trashBin->position = car->getPosition() + glm::vec3(offsetX, height, offsetY);
        } else if (normalizeAngle(rotation) == 90.0f) {
            offsetX = -4.0f;
            trashBin->position = car->getPosition() + glm::vec3(offsetX, height, offsetY);
        } else if (normalizeAngle(rotation) == 180.0f) {
            offsetY = 4.0f;
            trashBin->position = car->getPosition() + glm::vec3(offsetX, height, offsetY);
        } else if (normalizeAngle(rotation) == 270.0f) {
            offsetX = 4.0f;
            trashBin->position = car->getPosition() + glm::vec3(offsetX, height, offsetY);
        }
    }

    return true;
}



void Trailer::render(const Camera& camera) {
    if (crashed) return;
    shader->use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    shader->setUniform("ambientLight", ambientLightColor);

    shader->setUniform("material.ambient", glm::vec3(1.0f));
    shader->setUniform("material.diffuse", glm::vec3(1.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f));
    shader->setUniform("material.shininess", 64.0f);

    shader->setUniform("Texture", *texture);
    mesh->render();
}

void Trailer::renderDepth(ppgso::Shader& depthShader) {
    if (crashed) return;
    depthShader.use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    depthShader.setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}


ppgso::Shader* Trailer::getShader() const {
    return shader.get();
}

Car* Trailer::getParentCar() {
    return parentCar;
}