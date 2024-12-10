#ifndef CAR_H
#define CAR_H

#include "renderable.h"
#include "camera.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>


class Car : public Renderable {

    glm::vec3 direction;
    bool crashed = false;
    glm::vec3 boundingBox;

public:
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    glm::vec3 position;
    float scale = 1.0f;
    float rotation = 0.0f;
    static std::unique_ptr<ppgso::Shader> shader;
    static glm::vec3 ambientLightColor;
    bool atIntersection = false;
    float animationTime{};
    float rotationTimer;
    bool turning = false;
    float targetRotation = 0.0f;
    float turnSpeed = 180.0f;
    glm::vec3 startPosition;
    bool isTruck;
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);
    glm::vec3 force = glm::vec3(0.0f);


    Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);

    bool isAtIntersection();

    bool update(float dTime, Scene& scene) override;
    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;
    ppgso::Shader* getShader() const override;


    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return direction; }
    float getRotation() const { return rotation; }
    float getRotationTimer() const { return rotationTimer; }


    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }
    void simulateCollision(Car& other, Scene& scene);
    void checkCollision(Scene& scene);
};

#endif // CAR_H