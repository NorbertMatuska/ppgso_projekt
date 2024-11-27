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

public:
    ParticleWindow();

    void drawGridLines(const Grid& grid);

    void onKey(int key, int scanCode, int action, int mods) override;
    void onCursorPos(double xpos, double ypos) override;
    void onIdle() override;
};

#endif //PPGSO_PARTICLE_WINDOW_H
