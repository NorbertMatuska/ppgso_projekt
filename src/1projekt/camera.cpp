#include "camera.h"

#include <iostream>
#include <glm/gtx/euler_angles.hpp>

Camera::Camera(float fov, float ratio, float near, float far) {
    projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
    position = glm::vec3(0.0f, 5.0f, 20.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    update();
}

void Camera::update() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    target = position + front;
    viewMatrix = glm::lookAt(position, target, up);
}
