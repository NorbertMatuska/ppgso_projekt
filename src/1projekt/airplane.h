#ifndef PPGSO_AIRPLANE_H
#define PPGSO_AIRPLANE_H

#include "renderable.h"
#include "animation_curve.h"
#include <ppgso/ppgso.h>
#include <memory>
#include <glm/vec3.hpp>

class Airplane final : public Renderable {
    static std::unique_ptr<ppgso::Shader> shader;

    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Texture> texture;

    float scale = 1.0f;

public:
    static glm::vec3 ambientLightColor;
    glm::vec3 position;
    glm::vec3 rotation; // euler angles: pitch, yaw, roll in degrees

    AnimationCurve animationCurve;
    float currentTime = 0.0f;
    float duration = 0.0f;

    Airplane();

    bool update(float dTime, Scene& scene) override;
    void render(const Camera& camera) override;
    void renderDepth(ppgso::Shader& depthShader) override;
    ppgso::Shader* getShader() const override;

    void setScale(float newScale) { scale = newScale; }

private:
    void finalizeAnimation();
};

#endif // PPGSO_AIRPLANE_H
