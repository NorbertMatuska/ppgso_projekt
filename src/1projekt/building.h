#ifndef PPGSO_BUILDING_H
#define PPGSO_BUILDING_H

#include "renderable.h"
#include "car.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>

class Building final : public Renderable {
    static std::unique_ptr<ppgso::Shader> shader;

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;

    float scale = 1.0f;
    float rotation = 0.0f;

public:
    static glm::vec3 ambientLightColor;
    glm::vec3 position;

    Building(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);

    bool update(float dTime, Scene& scene) override;
    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;
    ppgso::Shader* getShader() const override;

    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }
};

#endif // PPGSO_BUILDING_H
