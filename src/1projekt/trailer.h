#ifndef TRAILER_H
#define TRAILER_H

#include "renderable.h"
#include "camera.h"
#include "building.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>

#include "car.h"

class Trailer : public Renderable {
    glm::vec3 direction;
    bool crashed = false;
    glm::vec3 boundingBox;

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
    Car* parentCar = nullptr;
    glm::vec3 relativePosition;
    Building* trashBin = nullptr;

    Trailer(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);

    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;
    bool update(float dTime, Scene& scene) override;

    ppgso::Shader* getShader() const override;

    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    glm::vec3 getPosition() const { return position; }

    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }
    void attachToCar(Car* car) { parentCar = car; }
    void attachTrashBin(Building* bin) { trashBin = bin; }
    Car* getParentCar();
};

#endif // TRAILER_H
