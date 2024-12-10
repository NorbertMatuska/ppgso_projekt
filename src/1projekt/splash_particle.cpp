#include "splash_particle.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> SplashParticle::mesh;
std::unique_ptr<ppgso::Shader> SplashParticle::shader;

SplashParticle::SplashParticle(glm::vec3 p, glm::vec3 s, glm::vec3 c, float lt) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    position = p;
    speed = s;
    color = c;
    age = 0.0f;
    lifetime = lt;
}

ppgso::Shader* SplashParticle::getShader() const {
    return shader.get();
}

void SplashParticle::renderDepth(ppgso::Shader& depthShader) {
}

bool SplashParticle::update(float dTime, Scene &scene) {
    position += speed * dTime;
    age += dTime;

    speed.y -= 9.8f * dTime;

    if (age >= lifetime)
        return false;

    float scale = 0.05f * (1.0f - age / lifetime);
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    return true;
}

void SplashParticle::render(const Camera& camera) {
    shader->use();
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    shader->setUniform("OverallColor", color);
    mesh->render();
}
