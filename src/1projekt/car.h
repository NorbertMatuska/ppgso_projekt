#ifndef CAR_H
#define CAR_H

#include "renderable.h"
#include "camera.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp> // For rotating direction vectors
#include <string>


class Car : public Renderable {

    glm::vec3 direction; // Current movement direction
    bool crashed = false; // Whether the car is crashed
    glm::vec3 boundingBox; // Bounding box size for collision detection

public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    float scale = 1.0f;
    float rotation = 0.0f;
    static std::unique_ptr<ppgso::Shader> shader;
    glm::vec3 position;
    static glm::vec3 ambientLightColor;
    bool atIntersection = false;
    float animationTime{};
    float rotationTimer;

    // Constructor
    Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);

    bool isAtIntersection();

    // Overrides
    bool update(float dTime, Scene& scene);
    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;
    //void Car::simulateCollision(Car& other);

    // Shader getter
    ppgso::Shader* getShader() const override;

    // Position and direction control
    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return direction; }
    float getRotation() const { return rotation; }
    float getRotationTimer() const { return rotationTimer; }

    // Setters
    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }

    // Collision detection
    //void Car::checkCollision();
};

#endif // CAR_H