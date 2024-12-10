#include "airplane.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <iostream>
#include <cmath>

std::unique_ptr<ppgso::Shader> Airplane::shader;
glm::vec3 Airplane::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);

Airplane::Airplane()
    : position(0.0f, 50.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>("models/plane.obj");
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("models/plane.bmp"));

    float A = 70.0f;
    float B = 70.0f;
    float totalTime = 40.0f;
    float stepAngle = 22.5f;
    std::vector<float> barrelRollAngles = {45.0f, 135.0f, 225.0f, 315.0f};
    float barrelRollDuration = 2.0f;

    auto addKeyframeAtAngle = [&](float angleDegrees) {
        float angleRad = glm::radians(angleDegrees);
        float x = A * sin(angleRad);
        float z = B * sin(angleRad) * cos(angleRad);
        glm::vec3 pos(x + 20.0f, 50.0f, z + 20.0f);

        float dx_dt = A * cos(angleRad);
        float dz_dt = B * (cos(angleRad) * cos(angleRad) - sin(angleRad) * sin(angleRad));
        float yaw = glm::degrees(atan2(dx_dt, dz_dt));
        if (yaw < 0.0f) yaw += 360.0f;

        bool isBarrelRollTrigger = std::find(barrelRollAngles.begin(), barrelRollAngles.end(), angleDegrees) != barrelRollAngles.end();

        float time = (angleDegrees / 360.0f) * totalTime;
        if (isBarrelRollTrigger) {
            animationCurve.addKeyframe(Keyframe(time - barrelRollDuration / 2.0f, pos, glm::vec3(0.0f, yaw, 0.0f)));
            animationCurve.addKeyframe(Keyframe(time, pos, glm::vec3(0.0f, yaw, 360.0f)));
            animationCurve.addKeyframe(Keyframe(time + barrelRollDuration / 2.0f, pos, glm::vec3(0.0f, yaw, 0.0f)));
        } else {
            animationCurve.addKeyframe(Keyframe(time, pos, glm::vec3(0.0f, yaw, 0.0f)));
        }
    };

    for (float angleDeg = 0.0f; angleDeg <= 360.0f; angleDeg += stepAngle) {
        addKeyframeAtAngle(angleDeg);
    }

    finalizeAnimation();
}

bool Airplane::update(float dTime, Scene& scene) {
    if (animationCurve.keyframes.empty()) {
        return true;
    }

    currentTime += dTime;
    if (currentTime > duration) {
        currentTime = fmod(currentTime, duration);
    }

    try {
        position = animationCurve.getPosition(currentTime);
        rotation = animationCurve.getRotation(currentTime);
    }
    catch (const std::exception& e) {
        std::cerr << "Animation error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void Airplane::render(const Camera& camera) {
    shader->use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    shader->setUniform("LightSpaceMatrix", lightSpaceMatrix);

    shader->setUniform("material.ambient", glm::vec3(1.0f));
    shader->setUniform("material.diffuse", glm::vec3(1.0f));
    shader->setUniform("material.specular", glm::vec3(1.0f));
    shader->setUniform("material.shininess", 64.0f);

    shader->setUniform("Texture", *texture);

    shader->setUniform("ShadowMap", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    mesh->render();
}

void Airplane::renderDepth(ppgso::Shader& depthShader) {
    depthShader.use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    depthShader.setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}

ppgso::Shader* Airplane::getShader() const {
    return shader.get();
}

void Airplane::finalizeAnimation() {
    if (!animationCurve.keyframes.empty()) {
        duration = animationCurve.keyframes.back().time;
    }
}
