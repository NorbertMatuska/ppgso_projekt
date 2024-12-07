#ifndef PPGSO_PARTICLE_WINDOW_H
#define PPGSO_PARTICLE_WINDOW_H

#include <ppgso/ppgso.h>
#include "renderable.h"
#include "camera.h"
#include "scene.h"
#include <map>
#include "grid.h"

class ParticleWindow : public ppgso::Window {
private:
    Scene scene;
    Camera camera;
    Scenegl scenegl;
    std::map<int, bool> keys;

    float lastX;
    float lastY;
    bool firstMouse;
    float sensitivity;

    GLuint depthMapFBO;
    GLuint depthMap;
    ppgso::Shader depthShader;
    glm::mat4 lightSpaceMatrix;
    void initShadowMap();
    void renderDepthMap();
    void renderSun(const glm::vec3& lightPos);
    void updateDynamicLights(ppgso::Shader& shader);
    std::vector<glm::vec3> lampPositions;

public:
    ParticleWindow();
    glm::vec3 sunDirection;

    void updateSunPosition(float dTime);
    void setLightingUniforms(ppgso::Shader& shader);
    void onKey(int key, int scanCode, int action, int mods) override;
    void onCursorPos(double xpos, double ypos) override;
    void onIdle() override;
};

#endif //PPGSO_PARTICLE_WINDOW_H
