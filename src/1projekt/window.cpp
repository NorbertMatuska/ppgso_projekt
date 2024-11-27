#include "window.h"
#include "plane.h"
#include "particle.h"
#include "splash_particle.h"
#include "building.h"
#include "grid.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#define SIZEx 1280
#define SIZEy 720

ParticleWindow::ParticleWindow() : Window{"task7_particles", SIZEx, SIZEy}, camera{60.0f, (float)width / (float)height, 0.1f, 100.0f}, lastX(width / 2.0f), lastY(height / 2.0f), firstMouse(true), sensitivity(0.1f) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);

    int n = 5;  // Number of 3x3 sub-grids along each dimension
    float cellSize = 10.0f;  // Size of each grid cell
    float subGridSize = 3 * cellSize;  // Size of a 3x3 sub-grid

    Grid grid(n * 3, n * 3, cellSize, glm::vec3(0.0f, 0.0f, 0.0f));
    drawGridLines(grid);  // Draw the grid in the background

    // Iterate through each sub-grid to place buildings
    for (int subGridRow = 0; subGridRow < n; ++subGridRow) {
        for (int subGridCol = 0; subGridCol < n; ++subGridCol) {
            glm::vec3 subGridOrigin = glm::vec3(subGridCol * subGridSize, 0.0f, subGridRow * subGridSize);

            // Generate a 3x3 pattern for the current sub-grid
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    if ((row == 1 && col == 1) || row == 1 || col == 1) {
                        continue; // Skip the center and roads (leave these as roads)
                    }

                    int buildingType = glm::linearRand(1, 4);  // Random building type
                    std::string modelPath, texturePath;

                    switch (buildingType) {
                        case 1:
                            modelPath = "models/building.obj";
                            texturePath = "models/texture.bmp";
                            break;
                        case 2:
                            modelPath = "models/building2.obj";
                            texturePath = "models/texture2.bmp";
                            break;
                        case 3:
                            modelPath = "models/building3.obj";
                            texturePath = "models/texture3.bmp";
                            break;
                        case 4:
                            modelPath = "models/building4.obj";
                            texturePath = "models/texture4.bmp";
                            break;
                    }

                    glm::vec3 cellPosition = grid.getCellPosition(row, col) + subGridOrigin;
                    auto newBuilding = std::make_unique<Building>(modelPath, cellPosition, texturePath);
                    scene.push_back(std::move(newBuilding));
                }
            }
        }
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// Function to draw the grid lines on the scene
void ParticleWindow::drawGridLines(const Grid& grid) {
    int rows = 10;  // Number of rows in the grid
    int cols = 10;  // Number of columns in the grid
    float cellSize = 5.0f;  // Size of each cell in world units
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);  // Starting point of the grid

    glBegin(GL_LINES);  // Begin drawing lines
    glColor3f(0.5f, 0.5f, 0.5f);  // Set the grid color (gray)

    // Draw horizontal lines
    for (int r = 0; r <= rows; ++r) {
        glm::vec3 start = origin + glm::vec3(0.0f, 0.0f, r * cellSize);
        glm::vec3 end = origin + glm::vec3(cols * cellSize, 0.0f, r * cellSize);
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
    }

    // Draw vertical lines
    for (int c = 0; c <= cols; ++c) {
        glm::vec3 start = origin + glm::vec3(c * cellSize, 0.0f, 0.0f);
        glm::vec3 end = origin + glm::vec3(c * cellSize, 0.0f, rows * cellSize);
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
    }

    glEnd();  // End drawing lines
}



void ParticleWindow::onKey(int key, int scanCode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        float x = glm::linearRand(-7.0f, 7.0f);
        float y = glm::linearRand(0.5f, 5.0f);
        float z = glm::linearRand(-7.0f, 7.0f);
        glm::vec3 position = glm::vec3(x, y, z);

        float r = glm::linearRand(0.2f, 1.0f);
        float g = glm::linearRand(0.2f, 1.0f);
        float b = glm::linearRand(0.2f, 1.0f);

        float spd = glm::linearRand(5.0f, 10.0f);
        for (int i = 0; i < 100; ++i) {
            glm::vec3 color = glm::vec3(r, g, b);
            glm::vec3 speed = glm::sphericalRand(spd);
            float lt = glm::linearRand(0.1f, 1.5f);

            auto particle = std::make_unique<SplashParticle>(position, speed, color, lt);
            scene.push_back(std::move(particle));
        }
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void ParticleWindow::onCursorPos(double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    camera.update();
}

void ParticleWindow::onIdle() {
    static auto time = (float)glfwGetTime();
    float dTime = (float)glfwGetTime() - time;
    time = (float)glfwGetTime();

    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float particleTime = 0.0f;
    particleTime += dTime;
    if (particleTime > 0.1f) {
        particleTime = 0.0f;
        float x = glm::linearRand(-10.0f, 10.0f);
        float y = 10.0f;
        float z = glm::linearRand(-10.0f, 10.0f);
        glm::vec3 position = glm::vec3(x, y, z);
        glm::vec3 speed = glm::vec3(0.0f, -5.0f, 0.0f);
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
        auto particle = std::make_unique<Particle>(position, speed, color);
        scene.push_back(std::move(particle));
    }

    float cameraSpeed = 10.0f;
    glm::vec3 forward = glm::normalize(camera.target - camera.position);
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

    if (keys[GLFW_KEY_W]) {
        camera.position += forward * cameraSpeed * dTime;
    }
    if (keys[GLFW_KEY_S]) {
        camera.position -= forward * cameraSpeed * dTime;
    }
    if (keys[GLFW_KEY_A]) {
        camera.position -= right * cameraSpeed * dTime;
    }
    if (keys[GLFW_KEY_D]) {
        camera.position += right * cameraSpeed * dTime;
    }

    camera.update();

    auto i = std::begin(scene);
    while (i != std::end(scene)) {
        auto obj = i->get();
        if (!obj->update(dTime, scene))
            i = scene.erase(i);
        else
            ++i;
    }

    for (auto& object : scene) {
        object->render(camera);
    }
}
