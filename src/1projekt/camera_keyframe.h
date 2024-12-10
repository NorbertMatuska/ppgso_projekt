// camera_keyframes.h
#ifndef PPGSO_CAMERA_KEYFRAMES_H
#define PPGSO_CAMERA_KEYFRAMES_H

#include "keyframe.h"
#include <glm/vec3.hpp>

struct CameraKeyframe {
    float time;
    glm::vec3 position;
    glm::vec3 target;

    CameraKeyframe(float t, const glm::vec3& pos, const glm::vec3& tar)
        : time(t), position(pos), target(tar) {}
};

#endif // PPGSO_CAMERA_KEYFRAMES_H
