#ifndef SKYBOX_H
#define SKYBOX_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <vector>
#include <string>
#include <memory>

class Skybox final : public Renderable {
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Mesh> mesh;
    GLuint cubemapTextureID;

    void loadCubemap(const std::vector<std::string>& faces);

public:
    Skybox(const std::vector<std::string>& faces);
    ~Skybox();

    bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
    ppgso::Shader* getShader() const override;
    void renderDepth(ppgso::Shader& depthShader) override;

};

#endif // SKYBOX_H
