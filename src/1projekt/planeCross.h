#ifndef PPGSO_PLANECROSS_H
#define PPGSO_PLANECROSS_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <memory>

class PlaneCross final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 position;
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    float rotation = 0.0f;

public:

    static glm::vec3 ambientLightColor;
    ppgso::Shader* getShader() const override;
    void renderDepth(ppgso::Shader& depthShader) override;


    explicit PlaneCross(const glm::vec3& position = {0.0f, 0.0f, 0.0f});

    void setRotation(float angle) { rotation = angle; }

    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
};

#endif //PPGSO_PLANECROSS_H
