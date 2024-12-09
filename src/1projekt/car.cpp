#include "car.h"
#include "camera.h"
#include "renderable.h"
#include "splash_particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <random>

std::unique_ptr<ppgso::Shader> Car::shader;
glm::vec3 Car::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);
static std::vector<Car*> allCars;

Car::Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : direction(1.0f, 0.0f, 0.0f), boundingBox(glm::vec3(1.0f)), position(initialPosition), startPosition(initialPosition) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>(objFilename);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));

    if (objFilename.find("truck") != std::string::npos) { isTruck = true; }
    else { isTruck = false; }

    allCars.push_back(this);
}

void Car::simulateCollision(Car& other, Scene& scene) {
    // Prevent multiple collision responses in the same frame
    if (crashed || other.crashed) return;

    // Mark both cars as crashed
    crashed = true;
    other.crashed = true;

    // Reset animation times
    animationTime = 0.0f;
    other.animationTime = 0.0f;

    // Define masses (assuming equal mass for simplicity)
    float mass1 = 1.0f;
    float mass2 = 1.0f;

    // Calculate the collision normal (normalized vector from other to this)
    glm::vec3 collisionNormal = glm::normalize(position - other.position);

    // Relative velocity
    glm::vec3 relativeVelocity = direction - other.direction;

    // Velocity along the collision normal
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    // If velocities are separating, do not resolve
    if (velocityAlongNormal > 0)
        return;

    // Coefficient of restitution (1.0 for perfectly elastic collision)
    float restitution = 0.8f; // Adjust between 0 (inelastic) and 1 (elastic)

    // Calculate impulse scalar
    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (1 / mass1) + (1 / mass2);

    // Apply impulse to the directions (velocities)
    glm::vec3 impulse = impulseScalar * collisionNormal;
    direction += (impulse / mass1);
    other.direction -= (impulse / mass2);

    // Normalize directions to maintain consistent speed
    direction = glm::normalize(direction);
    other.direction = glm::normalize(other.direction);

    // Update rotations based on new directions
    rotation = glm::degrees(atan2(direction.z, direction.x));
    other.rotation = glm::degrees(atan2(other.direction.z, other.direction.x));

    // Generate splash particles at the point of collision
    glm::vec3 collisionPoint = (position + other.position) / 2.0f;
    int particlesPerCar = 100;

    auto generateParticles = [&](const glm::vec3& basePosition, const glm::vec3& dir) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> offsetDist(-1.0f, 1.0f);
        std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);
        std::uniform_real_distribution<float> speedDist(5.0f, 10.0f);
        std::uniform_real_distribution<float> lifetimeDist(0.5f, 1.5f);

        for (int i = 0; i < particlesPerCar; ++i) {
            // Slightly offset the collision point for particle distribution
            glm::vec3 particlePosition = basePosition + glm::vec3(
                offsetDist(gen) * 0.5f,
                glm::linearRand(0.0f, 2.0f),
                offsetDist(gen) * 0.5f
            );

            // Random color for the particle
            glm::vec3 color = glm::vec3(colorDist(gen), colorDist(gen), colorDist(gen));

            // Particle velocity influenced by collision direction
            glm::vec3 velocity = glm::normalize(dir) * speedDist(gen) + glm::sphericalRand(2.0f);

            float lifetime = lifetimeDist(gen);

            auto particle = std::make_unique<SplashParticle>(particlePosition, velocity, color, lifetime);
            scene.push_back(std::move(particle));
        }
    };

    // Generate particles for both cars
    generateParticles(collisionPoint, direction);
    generateParticles(collisionPoint, other.direction);
}


void Car::checkCollision(Scene& scene) {
    for (Car* other : allCars) {
        if (other == this || other->crashed) continue;

        if (glm::abs(position.x - other->position.x) < boundingBox.x &&
            glm::abs(position.y - other->position.y) < boundingBox.y &&
            glm::abs(position.z - other->position.z) < boundingBox.z) {
            simulateCollision(*other, scene);
            }
    }
}


bool Car::update(float dTime, Scene& scene) {
    if (crashed) {
        animationTime += dTime;
        if (animationTime <= 1.0f) {
            position += direction * dTime * 3.0f;
        } else {
            direction = glm::vec3(0.0f);
        }

        if (animationTime >= 5.0f) {
            crashed = false;
            animationTime = 0.0f;
            position = startPosition;
            direction = glm::vec3(1.0f, 0.0f, 0.0f);
            if (isTruck) {
                rotation = 90.0f;
            } else {
                rotation = 0.0f;
            }
            turning = false;
            atIntersection = false;
        }

        return true;
    }

    if (turning) {
        if (glm::abs(rotation - targetRotation) > 0.1f) {
            float rotationStep = turnSpeed * dTime;
            if (glm::abs(rotation - targetRotation) < rotationStep) {
                rotation = targetRotation;
                turning = false;
            } else {
                rotation += (targetRotation > rotation) ? rotationStep : -rotationStep;
            }
        }
    }

    position += direction * dTime * 5.0f;

    checkCollision(scene);

    float intersectionThreshold = 1.0f;
    bool nearIntersection =
        (glm::mod(position.x - 10.0f, 30.0f) < intersectionThreshold ||
         glm::mod(position.x - 10.0f, 30.0f) > (30.0f - intersectionThreshold)) &&
        (glm::mod(position.z - 10.0f, 30.0f) < intersectionThreshold ||
         glm::mod(position.z - 10.0f, 30.0f) > (30.0f - intersectionThreshold));

    int n = 3;
    float mapEnd = 10.0f + (n - 1) * 30.0f + 10.0f;

    if (position.x < 0.0f || position.x > mapEnd || position.z < 0.0f || position.z > mapEnd) {
        direction = -direction;
        rotation += 180.0f;
        return true;
    }

    if (nearIntersection && !atIntersection) {
        atIntersection = true;
        int turnChoice = glm::linearRand(0, 2);
        switch (turnChoice) {
            case 0:
                // go straight
                break;
            case 1:
                direction = glm::vec3(-direction.z, 0.0f, direction.x);
                targetRotation = rotation - 90.0f;  // left turn
                turning = true;
                break;
            case 2:
                direction = glm::vec3(direction.z, 0.0f, -direction.x);
                targetRotation = rotation + 90.0f;  // right turn
                turning = true;
                break;
        }
    } else if (!nearIntersection && atIntersection) {
        atIntersection = false;
    }

    return true;
}



void Car::render(const Camera& camera) {
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

void Car::renderDepth(ppgso::Shader& depthShader) {
    if (crashed) return;
    depthShader.use();

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    depthShader.setUniform("ModelMatrix", modelMatrix);
    mesh->render();
}


ppgso::Shader* Car::getShader() const {
    return shader.get();
}