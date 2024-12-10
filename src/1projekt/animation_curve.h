#ifndef PPGSO_ANIMATION_CURVE_H
#define PPGSO_ANIMATION_CURVE_H

#include "keyframe.h"
#include <vector>
#include <glm/vec3.hpp>
#include <algorithm>
#include <stdexcept>


class AnimationCurve {
public:
    std::vector<Keyframe> keyframes;

    void addKeyframe(const Keyframe& keyframe) {
        keyframes.push_back(keyframe);
        std::sort(keyframes.begin(), keyframes.end(),
                  [](const Keyframe& a, const Keyframe& b) { return a.time < b.time; });
    }

    glm::vec3 getPosition(float time) const {
        if (keyframes.empty()) {
            throw std::runtime_error("No keyframes available for position interpolation.");
        }

        if (time <= keyframes.front().time) {
            return keyframes.front().position;
        }

        if (time >= keyframes.back().time) {
            return keyframes.back().position;
        }

        size_t i = 0;
        while (i < keyframes.size() - 1 && keyframes[i + 1].time < time) {
            ++i;
        }

        // keyframes for catmull-rom spline
        const Keyframe& k0 = (i == 0) ? keyframes[i] : keyframes[i - 1];
        const Keyframe& k1 = keyframes[i];
        const Keyframe& k2 = keyframes[i + 1];
        const Keyframe& k3 = (i + 2 < keyframes.size()) ? keyframes[i + 2] : keyframes[i + 1];

        float t = (time - k1.time) / (k2.time - k1.time);

        // catmull-rom spline interpolation for pos
        return 0.5f * (
            (2.0f * k1.position) +
            (-k0.position + k2.position) * t +
            (2.0f * k0.position - 5.0f * k1.position + 4.0f * k2.position - k3.position) * t * t +
            (-k0.position + 3.0f * k1.position - 3.0f * k2.position + k3.position) * t * t * t
        );
    }

    glm::vec3 getRotation(float time) const {
        if (keyframes.empty()) {
            throw std::runtime_error("No keyframes available for rotation interpolation.");
        }

        if (time <= keyframes.front().time) {
            return keyframes.front().rotation;
        }

        if (time >= keyframes.back().time) {
            return keyframes.back().rotation;
        }

        size_t i = 0;
        while (i < keyframes.size() - 1 && keyframes[i + 1].time < time) {
            ++i;
        }

        // keyframes for catmull-rom spline
        const Keyframe& k0 = (i == 0) ? keyframes[i] : keyframes[i - 1];
        const Keyframe& k1 = keyframes[i];
        const Keyframe& k2 = keyframes[i + 1];
        const Keyframe& k3 = (i + 2 < keyframes.size()) ? keyframes[i + 2] : keyframes[i + 1];

        float t = (time - k1.time) / (k2.time - k1.time);

        glm::vec3 interpolatedRotation;
        for (int axis = 0; axis < 3; ++axis) {
            float r0 = k0.rotation[axis];
            float r1 = k1.rotation[axis];
            float r2 = k2.rotation[axis];
            float r3 = k3.rotation[axis];

            // catmull-rom spline interpolation for each euler angle
            interpolatedRotation[axis] = 0.5f * (
                (2.0f * r1) +
                (-r0 + r2) * t +
                (2.0f * r0 - 5.0f * r1 + 4.0f * r2 - r3) * t * t +
                (-r0 + 3.0f * r1 - 3.0f * r2 + r3) * t * t * t
            );
        }

        return interpolatedRotation;
    }
};

#endif // PPGSO_ANIMATION_CURVE_H
