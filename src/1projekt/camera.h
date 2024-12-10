#ifndef PPGSO_CAMERA_H
#define PPGSO_CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float yaw;
    float pitch;

    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 150.0f);

    void update();
};

#endif //PPGSO_CAMERA_H
