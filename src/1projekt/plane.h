#ifndef PPGSO_PLANE_H
#define PPGSO_PLANE_H


#include "renderable.h"
#include <ppgso/ppgso.h>
#include <memory>

class Plane final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

public:
    Plane();

    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
};

#endif //PPGSO_PLANE_H
