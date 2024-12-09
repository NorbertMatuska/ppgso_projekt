#include "car.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <random>

std::unique_ptr<ppgso::Shader> Car::shader;
glm::vec3 Car::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);
static std::vector<Car*> allCars;

Car::Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : position(initialPosition), startPosition(initialPosition), direction(1.0f, 0.0f, 0.0f), boundingBox(glm::vec3(1.0f)) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>(objFilename);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));

    allCars.push_back(this);
}

void Car::simulateCollision(Car& other) {
    crashed = true;
    other.crashed = true;

    animationTime = 0.0f;
    other.animationTime = 0.0f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angleDist(0, 360);

    float randomAngle1 = glm::radians(angleDist(gen));
    float randomAngle2 = glm::radians(angleDist(gen));

    direction = glm::vec3(cos(randomAngle1), 0.0f, sin(randomAngle1));
    other.direction = glm::vec3(cos(randomAngle2), 0.0f, sin(randomAngle2));
}


void Car::checkCollision() {
    for (Car* other : allCars) {
        if (other == this || other->crashed) continue;

        if (glm::abs(position.x - other->position.x) < boundingBox.x &&
            glm::abs(position.y - other->position.y) < boundingBox.y &&
            glm::abs(position.z - other->position.z) < boundingBox.z) {
            simulateCollision(*other);
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
            rotation = 0.0f;
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

    //position += direction * dTime * 5.0f;

    checkCollision();

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