#ifndef PPGSO_BUILDING_H
#define PPGSO_BUILDING_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <memory>

class Building final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    Building();

    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
};

#endif //PPGSO_BUILDING_H
