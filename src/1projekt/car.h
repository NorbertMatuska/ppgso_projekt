#ifndef CAR_H
#define CAR_H

#include "renderable.h"
#include "camera.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp> // For rotating direction vectors
#include <string>

class Car final : public Renderable {
    static std::unique_ptr<ppgso::Shader> shader;

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 direction; // Current movement direction
    float scale = 1.0f;
    float rotation = 0.0f; // Rotation in degrees
    bool crashed = false; // Whether the car is crashed
    glm::vec3 boundingBox; // Bounding box size for collision detection

public:
    glm::vec3 position;
    static glm::vec3 ambientLightColor;

    // Constructor
    Car(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);

    bool isAtIntersection();

    // Overrides
    bool update(float dTime, Scene& scene);
    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;

    // Shader getter
    ppgso::Shader* getShader() const override;

    // Position and direction control
    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    glm::vec3 getPosition() const { return position; }

    // Setters
    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }

    // Collision detection
    bool checkCollision(const Car& other) const;

    // Collision simulation
    void simulateCollision(Car& other);
};

#endif // CAR_H