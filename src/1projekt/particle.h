#ifndef PPGSO_PARTICLE_H
#define PPGSO_PARTICLE_H

#include "renderable.h"
#include <ppgso/ppgso.h>
#include <glm/glm.hpp>
#include <memory>

class Particle final : public Renderable {
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 color;
    glm::vec3 wind;

    float age;
    float lifetime;
    glm::mat4 modelMatrix;

public:
    Particle(glm::vec3 p, glm::vec3 s, glm::vec3 c);

    virtual bool update(float dTime, Scene &scene) override;
    void render(const Camera& camera) override;
    ppgso::Shader* getShader() const override;
    void renderDepth(ppgso::Shader& depthShader) override;
    void setWind(const glm::vec3& wind);
};

#endif //PPGSO_PARTICLE_H
