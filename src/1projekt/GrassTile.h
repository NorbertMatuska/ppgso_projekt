#ifndef PPGSO_GRASSTILE_H
#define PPGSO_GRASSTILE_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <memory>

class GrassTile final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 position;
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

public:
    explicit GrassTile(const glm::vec3& position = {0.0f, -0.1f, 0.0f}, const glm::vec3& scale = {1.0f, 1.0f, 1.0f});

    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
    ppgso::Shader* getShader() const override;
    void renderDepth(ppgso::Shader& depthShader) override;

};

#endif //PPGSO_GRASSTILE_H
