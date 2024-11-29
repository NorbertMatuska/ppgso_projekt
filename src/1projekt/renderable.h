#ifndef PPGSO_RENDERABLE_H
#define PPGSO_RENDERABLE_H

#include <list>
#include <memory>
#include <ppgso.h>

class Camera; // Forward declaration

class Renderable; // Forward declaration
using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
    virtual ~Renderable() = default;

    virtual void render(const Camera& camera) = 0;
    virtual bool update(float dTime, Scene &scene) = 0;
    virtual ppgso::Shader* getShader() const = 0;
    virtual void renderDepth(ppgso::Shader& depthShader) = 0;

    static glm::mat4 lightSpaceMatrix;
    static GLuint depthMap;

};

#endif //PPGSO_RENDERABLE_H
