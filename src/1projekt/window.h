#ifndef PPGSO_PARTICLE_WINDOW_H
#define PPGSO_PARTICLE_WINDOW_H

#include <ppgso/ppgso.h>
#include "renderable.h"
#include "camera.h"
#include "scene.h"
#include <map>
#include "grid.h"
#include "PostProcessor.h"
#include "car.h"
#include "animation_curve.h"
#include "camera_keyframe.h"
#include "keyframe.h"

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

    float particleSpawnTimer = 0.0f;
    const float particleSpawnInterval = 0.1f;
    const float spawnRadius = 10.0f;
    glm::vec3 wind;
    float windChangeTimer = 0.0f;
    const float windChangeInterval = 5.0f;

    GLuint depthMapFBO;
    GLuint depthMap;
    ppgso::Shader depthShader;
    glm::mat4 lightSpaceMatrix;
    void initShadowMap();
    void renderDepthMap();
    void renderSun(const glm::vec3& lightPos);
    void updateDynamicLights(ppgso::Shader& shader);
    std::vector<glm::vec3> lampPositions;

    bool isCameraAnimating = false;
    float cameraAnimationTime = 0.0f;
    float cameraAnimationDuration = 0.0f;
    AnimationCurve cameraAnimationCurve;
    void initializeCameraAnimation();
    void updateCameraAnimation(float dTime);

public:
    ParticleWindow();
    glm::vec3 sunDirection;
    std::unique_ptr<PostProcessor> postProcessor;

    void updateSunPosition(float dTime);
    void setLightingUniforms(ppgso::Shader& shader);
    void onKey(int key, int scanCode, int action, int mods) override;
    void onCursorPos(double xpos, double ypos) override;
    void onIdle() override;
    void addCarLights(ppgso::Shader& shader, const glm::vec3& carPosition);
};

#endif //PPGSO_PARTICLE_WINDOW_H
