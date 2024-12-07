#include "car.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Shader> Car::shader;
glm::vec3 Car::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);
float animationTime;
bool atIntersection = false;

Car::Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : position(initialPosition), direction(1.0f, 0.0f, 0.0f), boundingBox(glm::vec3(1.0f)) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>(objFilename);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));
}

void Car::simulateCollision(Car& other) {
    crashed = true;
    other.crashed = true;

    animationTime = 0.0f;
}


bool Car::update(float dTime, Scene& scene) {
    if (crashed) return false;

    // Update position
    position += direction * dTime * 5.0f;

    // Detect if the car is at an intersection
    float intersectionThreshold = 1.0f; // Threshold for intersection detection

    bool nearIntersection =
        (glm::mod(position.x - 10.0f, 30.0f) < intersectionThreshold ||
         glm::mod(position.x - 10.0f, 30.0f) > (30.0f - intersectionThreshold)) &&
        (glm::mod(position.z - 10.0f, 30.0f) < intersectionThreshold ||
         glm::mod(position.z - 10.0f, 30.0f) > (30.0f - intersectionThreshold));

    // Number of intersections along one axis
    int n = 3; // Example: adjust as needed
    float mapEnd = 10.0f + (n - 1) * 30.0f + 10.0f; // End boundary for both x and z

    // Check if car is out of bounds
    if (position.x < 0.0f || position.x > mapEnd || position.z < 0.0f || position.z > mapEnd) {
        // Turn 180 degrees
        direction = -direction;
        rotation += 180.0f; // Rotate the car 180 degrees (if you are tracking its visual orientation)
        return true;
    }

    if (nearIntersection && !atIntersection) {
        atIntersection = true;

        // Randomly choose a direction at the intersection
        int turnChoice = glm::linearRand(0, 2); // 0 = straight, 1 = left, 2 = right
        switch (turnChoice) {
            case 0: // Go straight
                break;
            case 1: // Turn left
                direction = glm::vec3(-direction.z, 0.0f, direction.x); // Rotate 90 degrees left
            rotation -= 90.0f;
            break;
            case 2: // Turn right
                direction = glm::vec3(direction.z, 0.0f, -direction.x); // Rotate 90 degrees right
            rotation += 90.0f;
            break;
        }
    } else if (!nearIntersection && atIntersection) {
        // Reset the state when leaving the intersection
        atIntersection = false;
    }

    return true;
}




void Car::render(const Camera& camera) {
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