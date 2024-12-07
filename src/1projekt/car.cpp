#include "car.h"
#include "camera.h"
#include "renderable.h"
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Shader> Car::shader;
glm::vec3 Car::ambientLightColor = glm::vec3(0.8f, 0.7f, 0.6f);

Car::Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename)
        : position(initialPosition), direction(1.0f, 0.0f, 0.0f), boundingBox(glm::vec3(1.0f)) {
    if (!shader) {
        shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    }

    mesh = std::make_unique<ppgso::Mesh>(objFilename);
    texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(textureFilename));
}

bool Car::update(float dTime, Scene& scene) {
    if (crashed) return false;

    // Update position
    position += direction * dTime;
    //std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;

        int turnChoice = glm::linearRand(0, 2); // 0 = straight, 1 = left, 2 = right

    if(position.x > 38){
        switch (turnChoice) {
            case 0: // Go straight
                std::cout << "Go straight"  << std::endl;
                    break;
            case 1: // Turn left
                std::cout << "Go left"  << std::endl;
                direction = glm::vec3(-direction.z, 0.0f, direction.x); // Rotate 90 degrees left
            rotation -= 90.0f; // Adjust rotation angle
            break;
            case 2: // Turn right
                std::cout << "Go right"  << std::endl;
                direction = glm::vec3(direction.z, 0.0f, -direction.x); // Rotate 90 degrees right
            rotation += 90.0f; // Adjust rotation angle
            break;
        }
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

bool Car::checkCollision(const Car& other) const {
    // Basic AABB collision detection
    glm::vec3 delta = position - other.position;
    glm::vec3 combinedHalfSize = boundingBox + other.boundingBox;
    return glm::abs(delta.x) <= combinedHalfSize.x &&
           glm::abs(delta.y) <= combinedHalfSize.y &&
           glm::abs(delta.z) <= combinedHalfSize.z;
}

void Car::simulateCollision(Car& other) {
    // Mark both cars as crashed
    crashed = true;
    other.crashed = true;

    // Optionally, adjust positions to simulate collision effect
    position -= direction * 0.1f;
    other.position -= other.direction * 0.1f;

    // Optionally, you can add a visual effect or sound here
}

ppgso::Shader* Car::getShader() const {
    return shader.get();
}