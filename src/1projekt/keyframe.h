#ifndef PPGSO_KEYFRAME_H
#define PPGSO_KEYFRAME_H

#include <glm/vec3.hpp>

struct Keyframe {
    float time;
    glm::vec3 position;
    glm::vec3 rotation;

    Keyframe(float t, const glm::vec3& pos, const glm::vec3& rot)
        : time(t), position(pos), rotation(rot) {}
};

#endif // PPGSO_KEYFRAME_H
