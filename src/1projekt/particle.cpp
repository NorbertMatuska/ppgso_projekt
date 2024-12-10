#include "particle.h"
#include "camera.h"
#include "splash_particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Shader> Particle::shader;

Particle::Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
    position = p;
    speed = s;
    color = c;
    age = 0.0f;
    lifetime = 5.0f;
    wind = glm::vec3(0.0f);
}

ppgso::Shader* Particle::getShader() const {
    return shader.get();
}

void Particle::renderDepth(ppgso::Shader& depthShader) {
}

bool Particle::update(float dTime, Scene &scene) {
    speed += wind * dTime;

    position += speed * dTime;
    age += dTime;

    if (position.y <= 0.5f) {
        const int NUM_SPLASH_PARTICLES = 100;
        for (int i = 0; i < NUM_SPLASH_PARTICLES; ++i) {
            float angle = glm::linearRand(0.0f, glm::two_pi<float>());
            float speedValue = glm::linearRand(0.1f, 0.5f);
            glm::vec3 splashSpeed = glm::vec3(cos(angle), 0.5f, sin(angle)) * speedValue;
            glm::vec3 splashColor = glm::vec3(0.0f, 0.0f, 1.0f);
            auto splashParticle = std::make_unique<SplashParticle>(position, splashSpeed, splashColor, 1.0f);
            scene.push_back(std::move(splashParticle));
        }
        return false;
    }

    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));

    return true;
}

void Particle::render(const Camera& camera) {
    shader->use();
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("ViewMatrix", camera.viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
    shader->setUniform("OverallColor", color);
    mesh->render();
}

void Particle::setWind(const glm::vec3& windVec) {
    wind = windVec;
}