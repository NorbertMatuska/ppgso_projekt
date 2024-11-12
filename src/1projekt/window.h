#ifndef PPGSO_PARTICLE_WINDOW_H
#define PPGSO_PARTICLE_WINDOW_H

#include <ppgso/ppgso.h>
#include "renderable.h"
#include "camera.h"
#include <map>

class ParticleWindow : public ppgso::Window {
private:
    Scene scene;
    Camera camera;
    std::map<int, bool> keys;

    float lastX;
    float lastY;
    bool firstMouse;
    float sensitivity;

public:
    ParticleWindow();

    void onKey(int key, int scanCode, int action, int mods) override;
    void onCursorPos(double xpos, double ypos) override;
    void onIdle() override;
};

#endif //PPGSO_PARTICLE_WINDOW_H
