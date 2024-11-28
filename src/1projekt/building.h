#ifndef PPGSO_BUILDING_H
#define PPGSO_BUILDING_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>

class Building final : public Renderable {
    static std::unique_ptr<ppgso::Shader> shader;

    // Instance-specific resources
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;
    glm::vec3 position;
    float scale = 1.0f;
    float rotation = 0.0f;

public:
    static glm::vec3 ambientLightColor;
    ppgso::Shader* getShader() const override;


    Building(const std::string& objFilename, const glm::vec3& initialPosition, const std::string& textureFilename);
    void setLightingUniforms(ppgso::Shader* shader, const Camera& camera);
    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
    void setScale(float newScale) { scale = newScale; }
    void setRotation(float angle) { rotation = angle; }
};

#endif //PPGSO_BUILDING_H
