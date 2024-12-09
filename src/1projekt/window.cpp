#include "window.h"
#include "plane.h"
#include "planeCross.h"
#include "particle.h"
#include "splash_particle.h"
#include "building.h"
#include "grid.h"
#include "GrassTile.h"
#include "SkyBox.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <set>
#include <shaders/depth_frag_glsl.h>
#include <shaders/depth_vert_glsl.h>
#include <chrono>
#include "PostProcessor.h"
#include <GL/glew.h>
#include "car.h"
#include <random>
#include "trailer.h"


#define SIZEx 1280
#define SIZEy 720
#define NR_POINT_LIGHTS 15
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
static std::vector<Car*> Cars;

ParticleWindow::ParticleWindow()
        : Window{"Project_Matuska_Pacuta", SIZEx, SIZEy},
          camera{60.0f, (float)width / (float)height, 0.1f, 100.0f},
          lastX(width / 2.0f), lastY(height / 2.0f), firstMouse(true), sensitivity(0.1f),
          depthShader(depth_vert_glsl, depth_frag_glsl) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);

    postProcessor = std::make_unique<PostProcessor>(SIZEx, SIZEy);
    sunDirection = glm::normalize(glm::vec3(-0.5f, -0.1f, 0.3f));
    initShadowMap();

    int n = 3;
    float cellSize = 10.0f;
    float subGridSize = 3 * cellSize;

    Grid grid(n * 3, n * 3, cellSize, glm::vec3(0.0f, 0.0f, 0.0f));
    Grid gridcars(n * 3, n * 3, cellSize, glm::vec3(0.0f, 0.4f, 0.0f));
    Grid gridtruck(n * 3, n * 3, cellSize, glm::vec3(0.0f, 0.8f, 0.0f));

    std::vector<std::string> skyboxFaces = {
            "skyboxes/vz_dawn_right.bmp",
            "skyboxes/vz_dawn_left.bmp",
            "skyboxes/vz_dawn_up.bmp",
            "skyboxes/vz_dawn_down.bmp",
            "skyboxes/vz_dawn_front.bmp",
            "skyboxes/vz_dawn_back.bmp"
    };

    auto skybox = std::make_unique<Skybox>(skyboxFaces);
    scene.push_back(std::move(skybox));

    auto grassTile = std::make_unique<GrassTile>(glm::vec3(0.0f, -0.1f, 0.0f), glm::vec3(200.0f));
    scene.push_back(std::move(grassTile));
/*
    auto roadblock1 = std::make_unique<Building>("models/roadblock.obj", grid.getCellPosition(0, 0), "models/roadblock.bmp");
    roadblock1->setScale(0.003f);
    roadblock1->setRotation(glm::linearRand(-30.0f, 30.0f));

    scene.push_back(std::move(roadblock1));
*/

    const int carCount = 10;

    int gridRows = 2;
    int gridCols = 2;

    float spawnPositions[carCount][2] = {
    {1,1},{1,5},{1,7}, {4,1},{4,4},{4,7},{7,7},{7,6},{1,3},{1,4}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(1, gridRows);
    std::uniform_int_distribution<> colDist(1, gridCols);

    for (int i = 0; i < carCount; ++i) {

        int randomRow = rowDist(gen) * 3 + 1;
        int randomCol = colDist(gen) * 3 + 1;

        std::string textureFile = (i % 2 == 0) ? "models/car.bmp" : "models/car2.bmp";

        //auto car = std::make_unique<Car>("models/car.obj", gridcars.getCellPosition(randomRow, randomCol), textureFile);
        auto car = std::make_unique<Car>("models/car.obj", gridcars.getCellPosition(spawnPositions[i][0], spawnPositions[i][1]), textureFile);
        car->setScale(0.030f);

        // Add car lights to the shader
        auto shader = car->getShader();
        shader->use();
        addCarLights(*shader, gridcars.getCellPosition(spawnPositions[i][0], spawnPositions[i][1]));

        Cars.push_back(car.get());
        scene.push_back(std::move(car));
    }

    auto truckPosition = gridtruck.getCellPosition(1, 2);
    auto trailerPosition = glm::vec3(truckPosition.x, truckPosition.y, truckPosition.z - 0.1);
    auto trashPosition = glm::vec3(truckPosition.x, truckPosition.y + 0.01, truckPosition.z - 0.1);

    auto truck = std::make_unique<Car>("models/truck.obj", truckPosition, "models/stainless-steel.bmp");
    truck->setScale(1.8f);
    truck->setRotation(90.0f);

    auto trailer = std::make_unique<Trailer>("models/trailer.obj", trailerPosition , "models/stainless-steel.bmp");
    trailer->setScale(1.8f);
    trailer->setRotation(90.0f);

    trailer->attachToCar(truck.get());

    auto trailerTrash = std::make_unique<Building>("models/trashbin.obj", trashPosition, "models/trashbin.bmp");
    trailerTrash->setScale(0.003);

    trailer->attachTrashBin(trailerTrash.get());

    scene.push_back(std::move(truck));
    scene.push_back(std::move(trailer));
    scene.push_back(std::move(trailerTrash));

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Iterate through each sub-grid to place buildings and roads
    for (int subGridRow = 0; subGridRow < n; ++subGridRow) {
        for (int subGridCol = 0; subGridCol < n; ++subGridCol) {
            glm::vec3 subGridOrigin = glm::vec3(subGridCol * subGridSize, 0.0f, subGridRow * subGridSize);

            // Generate a 3x3 pattern for the current sub-grid
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    glm::vec3 cellPosition = grid.getCellPosition(row, col) + subGridOrigin;

                    // --------------------------
                    // ROAD GENERATION
                    // --------------------------
                    if (row == 1 || col == 1) {

                        if (!(row == 1 && col == 1)){
                            auto road = std::make_unique<Plane>(cellPosition);
                            if (row == 1 && col != 1) {
                                road->setRotation(90.0f);
                            }
                            scene.push_back(std::move(road));

                            // --------------------------
                            // ROADBLOCK GENERATION
                            // --------------------------

                            // determining, if its top, bottom, left, right city border
                            bool isCityBorder = (subGridRow == 0 && row == 0) ||
                                                (subGridRow == n - 1 && row == 2) ||
                                                (subGridCol == 0 && col == 0) ||
                                                (subGridCol == n - 1 && col == 2);

                            if (isCityBorder) {
                                glm::vec3 roadblockPosition1 = cellPosition;
                                glm::vec3 roadblockPosition2 = cellPosition;

                                // aligning each road block to the end of the road
                                if (row == 0 || row == 2) {
                                    roadblockPosition1.z += (row == 0 ? -1.0f : 1.0f) * (cellSize / 2);
                                    roadblockPosition1.x -= cellSize / 4;
                                    roadblockPosition2.z += (row == 0 ? -1.0f : 1.0f) * (cellSize / 2);
                                    roadblockPosition2.x += cellSize / 4;
                                } else if (col == 0 || col == 2) {
                                    roadblockPosition1.x += (col == 0 ? -1.0f : 1.0f) * (cellSize / 2);
                                    roadblockPosition1.z -= cellSize / 4;
                                    roadblockPosition2.x += (col == 0 ? -1.0f : 1.0f) * (cellSize / 2);
                                    roadblockPosition2.z += cellSize / 4;
                                }

                                roadblockPosition1.x += glm::linearRand(-0.5f, 0.5f);
                                roadblockPosition1.z += glm::linearRand(-0.5f, 0.5f);
                                roadblockPosition2.x += glm::linearRand(-0.5f, 0.5f);
                                roadblockPosition2.z += glm::linearRand(-0.5f, 0.5f);

                                roadblockPosition1.y += 1.5f;
                                roadblockPosition2.y += 1.5f;

                                auto roadblock1 = std::make_unique<Building>("models/roadblock.obj", roadblockPosition1, "models/roadblock.bmp");
                                roadblock1->setScale(0.003f);
                                roadblock1->setRotation(glm::linearRand(-30.0f, 30.0f));

                                auto roadblock2 = std::make_unique<Building>("models/roadblock.obj", roadblockPosition2, "models/roadblock.bmp");
                                roadblock2->setScale(0.003f);
                                roadblock2->setRotation(glm::linearRand(-30.0f, 30.0f));

                                if (row == 0 || row == 2) {
                                    roadblock1->setRotation(90.0f + glm::linearRand(-30.0f, 30.0f));
                                    roadblock2->setRotation(90.0f + glm::linearRand(-30.0f, 30.0f));
                                }
                                scene.push_back(std::move(roadblock1));
                                scene.push_back(std::move(roadblock2));
                            }
                        }

                        // intersection road textures
                        if (row == 1 && col == 1) {
                            auto cross = std::make_unique<PlaneCross>(cellPosition);
                            scene.push_back(std::move(cross));
                            continue;
                        }

                        // --------------------------
                        // LAMPS AND TRASHBINS GENERATION
                        // --------------------------
                        float lampOffset = 5.0f;
                        float randomOffsetX = glm::linearRand(-4.0f, 4.0f);
                        float randomOffsetZ = glm::linearRand(-4.0f, 4.0f);
                        float binScale = 0.003f;

                        if (row == 1) {
                            glm::vec3 leftLampPos = cellPosition + glm::vec3(0.0f, 0.0f, -lampOffset);
                            glm::vec3 rightLampPos = cellPosition + glm::vec3(0.0f, 0.0f, lampOffset);
                            lampPositions.push_back(leftLampPos);
                            lampPositions.push_back(rightLampPos);

                            auto leftLamp = std::make_unique<Building>("models/lamp.obj", leftLampPos, "models/lamp.bmp");
                            leftLamp->setScale(0.002f);
                            leftLamp->setRotation(270.0f);
                            scene.push_back(std::move(leftLamp));

                            auto rightLamp = std::make_unique<Building>("models/lamp.obj", rightLampPos, "models/lamp.bmp");
                            rightLamp->setScale(0.002f);
                            rightLamp->setRotation(90.0f);
                            scene.push_back(std::move(rightLamp));

                            glm::vec3 leftTrashBinPos = cellPosition + glm::vec3(randomOffsetX, 0.25f, -lampOffset);
                            glm::vec3 rightTrashBinPos = cellPosition + glm::vec3(randomOffsetX, 0.25f, lampOffset);

                            if (glm::linearRand(0.0f, 1.0f) < 0.3f) {
                                auto leftTrashBin = std::make_unique<Building>("models/trashbin.obj", leftTrashBinPos, "models/trashbin.bmp");
                                leftTrashBin->setScale(binScale);
                                scene.push_back(std::move(leftTrashBin));
                            }

                            if (glm::linearRand(0.0f, 1.0f) < 0.3f) {
                                auto rightTrashBin = std::make_unique<Building>("models/trashbin.obj", rightTrashBinPos, "models/trashbin.bmp");
                                rightTrashBin->setScale(binScale);
                                scene.push_back(std::move(rightTrashBin));
                            }
                        }

                        if (col == 1) {
                            glm::vec3 topLampPos = cellPosition + glm::vec3(-lampOffset, 0.0f, 0.0f);
                            glm::vec3 bottomLampPos = cellPosition + glm::vec3(lampOffset, 0.0f, 0.0f);
                            lampPositions.push_back(topLampPos);
                            lampPositions.push_back(bottomLampPos);

                            auto topLamp = std::make_unique<Building>("models/lamp.obj", topLampPos, "models/lamp.bmp");
                            topLamp->setScale(0.002f);
                            scene.push_back(std::move(topLamp));

                            auto bottomLamp = std::make_unique<Building>("models/lamp.obj", bottomLampPos, "models/lamp.bmp");
                            bottomLamp->setScale(0.002f);
                            bottomLamp->setRotation(180.0f);
                            scene.push_back(std::move(bottomLamp));

                            glm::vec3 topTrashBinPos = cellPosition + glm::vec3(-lampOffset, 0.25f, randomOffsetZ);
                            glm::vec3 bottomTrashBinPos = cellPosition + glm::vec3(lampOffset, 0.25f, randomOffsetZ);

                            if (glm::linearRand(0.0f, 1.0f) < 0.3f) {
                                auto topTrashBin = std::make_unique<Building>("models/trashbin.obj", topTrashBinPos, "models/trashbin.bmp");
                                topTrashBin->setScale(binScale);
                                topTrashBin->setRotation(glm::linearRand(0.0f, 360.0f));
                                scene.push_back(std::move(topTrashBin));
                            }

                            if (glm::linearRand(0.0f, 1.0f) < 0.3f) {
                                auto bottomTrashBin = std::make_unique<Building>("models/trashbin.obj", bottomTrashBinPos, "models/trashbin.bmp");
                                bottomTrashBin->setScale(binScale);
                                bottomTrashBin->setRotation(glm::linearRand(0.0f, 360.0f));
                                scene.push_back(std::move(bottomTrashBin));
                            }
                        }
                        continue;
                    }

                    // --------------------------
                    // BUILDINGS GENERATION
                    // --------------------------

                    // rand texture and type
                    int buildingType = glm::linearRand(1, 7);
                    int randomTexture = glm::linearRand(1, 4);
                    std::string modelPath, texturePath;


                    switch (buildingType) {
                        case 1:
                            modelPath = "models/building.obj";
                            texturePath = (randomTexture == 1) ? "models/texture.bmp" :
                                          (randomTexture == 2) ? "models/texture2.bmp" :
                                          (randomTexture == 3) ? "models/texture3.bmp" : "models/texture7.bmp";
                            break;
                        case 2:
                            modelPath = "models/building2.obj";
                            texturePath = (randomTexture == 1) ? "models/texture.bmp" :
                                          (randomTexture == 2) ? "models/texture2.bmp" :
                                          (randomTexture == 3) ? "models/texture3.bmp" : "models/texture7.bmp";
                            break;
                        case 3:
                            modelPath = "models/building3.obj";
                            texturePath = (randomTexture == 1) ? "models/texture.bmp" :
                                          (randomTexture == 2) ? "models/texture2.bmp" :
                                          (randomTexture == 3) ? "models/texture3.bmp" : "models/texture7.bmp";
                            break;
                        case 4:
                            modelPath = "models/building4.obj";
                            texturePath = (randomTexture == 1) ? "models/texture.bmp" :
                                          (randomTexture == 2) ? "models/texture2.bmp" :
                                          (randomTexture == 3) ? "models/texture3.bmp" : "models/texture7.bmp";
                            break;
                        case 5:
                            modelPath = "models/building5.obj";
                            texturePath = "models/texture5.bmp";
                            break;
                        case 6:
                            modelPath = "models/building6.obj";
                            texturePath = "models/texture6.bmp";
                            break;
                        case 7:
                            modelPath = "models/building7.obj";
                            texturePath = (randomTexture == 1) ? "models/texture.bmp" :
                                          (randomTexture == 2) ? "models/texture2.bmp" :
                                          (randomTexture == 3) ? "models/texture3.bmp" : "models/texture7.bmp";
                            break;
                    }

                    auto newBuilding = std::make_unique<Building>(modelPath, cellPosition, texturePath);
                    if (buildingType == 5) {
                        newBuilding->setScale(0.85);
                    }
                    scene.push_back(std::move(newBuilding));
                }
            }
        }
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ParticleWindow::setLightingUniforms(ppgso::Shader& shader) {
    shader.setUniform("viewPos", camera.position);

    // Directional light
    shader.setUniform("dirLight.direction", sunDirection);
    shader.setUniform("dirLight.ambient", glm::vec3(0.4f, 0.35f, 0.3f));
    shader.setUniform("dirLight.diffuse", glm::vec3(0.8f, 0.7f, 0.6f));
    shader.setUniform("dirLight.specular", glm::vec3(1.0f, 0.9f, 0.8f));

    // Point light 1
    /*
    shader.setUniform("pointLights[0].position", glm::vec3(10.0f, 10.0f, 10.0f));
    shader.setUniform("pointLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));  // Increase ambient intensity
    shader.setUniform("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));  // Increase diffuse intensity
    shader.setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f)); // Max specular intensity
    shader.setUniform("pointLights[0].constant", 1.0f);
    shader.setUniform("pointLights[0].linear", 0.07f);     // Adjust attenuation
    shader.setUniform("pointLights[0].quadratic", 0.017f); // Adjust attenuation

    // Point light 2 (adjust as needed)
    shader.setUniform("pointLights[1].position", glm::vec3(-10.0f, 10.0f, -10.0f));
    shader.setUniform("pointLights[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setUniform("pointLights[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("pointLights[1].constant", 1.0f);
    shader.setUniform("pointLights[1].linear", 0.07f);
    shader.setUniform("pointLights[1].quadratic", 0.017f);

    //Spotlight (from the camera's perspective)
    shader.setUniform("spotLight.position", camera.position);
    shader.setUniform("spotLight.direction", glm::normalize(camera.target - camera.position));
    shader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader.setUniform("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.setUniform("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("spotLight.constant", 1.0f);
    shader.setUniform("spotLight.linear", 0.07f);
    shader.setUniform("spotLight.quadratic", 0.017f);
    */
}

void ParticleWindow::updateDynamicLights(ppgso::Shader& shader) {
    std::vector<glm::vec3> activeLights;
    float activationRadius = 25.0f; // Radius around the camera

    // Filter lamp lights based on distance from the camera
    for (const auto &lampPos: lampPositions) {
        if (glm::distance(camera.position, lampPos) <= activationRadius) {
            activeLights.push_back(lampPos);
        }
    }

    // Add car lights dynamically
    for (const auto& car : Cars) {
        glm::vec3 carPos = car->getPosition();
        if (glm::distance(camera.position, carPos) <= activationRadius) {
            activeLights.push_back(carPos + glm::vec3(0.5f, -1.0f, 1.4f)); // Front-left light
            activeLights.push_back(carPos + glm::vec3(-0.5f, -1.0f, 1.4f)); // Front-right light
        }
    }

    // Update uniforms for the active lights
    int lightCount = 0;
    for (const auto& activeLight : activeLights) {
        if (lightCount >= NR_POINT_LIGHTS) break;

        std::string index = "pointLights[" + std::to_string(lightCount) + "].";
        shader.setUniform(index + "position", activeLight);
        shader.setUniform(index + "ambient", glm::vec3(0.05f));
        shader.setUniform(index + "diffuse", glm::vec3(0.8f, 0.8f, 0.7f));
        shader.setUniform(index + "specular", glm::vec3(1.0f));
        shader.setUniform(index + "constant", 1.0f);
        shader.setUniform(index + "linear", 0.045f);
        shader.setUniform(index + "quadratic", 0.0075f);

        lightCount++;
    }
}


void ParticleWindow::initShadowMap() {
    // Generate the framebuffer
    glGenFramebuffers(1, &depthMapFBO);

    // Create the depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // Important for shadows
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Define the border color for when sampling outside the shadow map
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach the depth texture to the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // We don't need a color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ParticleWindow::renderDepthMap() {
    // Set the static depthMap in Renderable
    Renderable::depthMap = depthMap;

    // Set viewport to the size of the shadow map
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    // Bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Use the depth shader
    depthShader.use();
    depthShader.setUniform("LightSpaceMatrix", lightSpaceMatrix);

    // Render the scene with the depth shader
    for (auto& object : scene) {
        object->renderDepth(depthShader);
    }

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Restore viewport
    glViewport(0, 0, width, height);
}


void ParticleWindow::onKey(int key, int scanCode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        /*
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
         */
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

void ParticleWindow::updateSunPosition(float dTime) {
    static float sunAngle = 0.0f;

    sunAngle += dTime * 0.5;

    if (sunAngle > glm::two_pi<float>()) {
        sunAngle -= glm::two_pi<float>();
    }


    float radius = 500.0f;

    float x = cos(sunAngle) * radius;
    float y = 10.0f;
    float z = sin(sunAngle) * radius;

    // Update sunDirection with the new values
    sunDirection = glm::normalize(glm::vec3(x, y, z));

/*
    // day n night
    float x = sin(sunAngle);
    float y = cos(sunAngle);
    sunDirection = glm::normalize(glm::vec3(x, y, 0.0f));
    */

}

void ParticleWindow::onIdle() {
    static auto time = (float)glfwGetTime();
    float dTime = (float)glfwGetTime() - time;
    time = (float)glfwGetTime();

    // (Optional) Update sun direction if needed
    updateSunPosition(dTime);

    // Setup the light space matrix for shadows
    glm::vec3 lightPos = -sunDirection * 150.0f;
    float near_plane = 1.0f, far_plane = 200.0f;
    glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    Renderable::lightSpaceMatrix = lightSpaceMatrix;

    // 1. Render the depth map for shadows
    renderDepthMap();

    // 2. Begin rendering into our HDR framebuffer using the PostProcessor
    postProcessor->BeginRender();
    // Now everything drawn goes into the HDR FBO

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handle camera input
    float cameraSpeed = 10.0f;
    glm::vec3 forward = glm::normalize(camera.target - camera.position);
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));

    if (keys[GLFW_KEY_W]) camera.position += forward * cameraSpeed * dTime;
    if (keys[GLFW_KEY_S]) camera.position -= forward * cameraSpeed * dTime;
    if (keys[GLFW_KEY_A]) camera.position -= right * cameraSpeed * dTime;
    if (keys[GLFW_KEY_D]) camera.position += right * cameraSpeed * dTime;

    camera.update();

    // Update objects
    for (auto it = scene.begin(); it != scene.end();) {
        if (!(*it)->update(dTime, scene))
            it = scene.erase(it);
        else
            ++it;
    }

    // Set lighting uniforms for all shaders
    std::set<ppgso::Shader*> processedShaders;
    for (auto& object : scene) {
        auto shader = object->getShader();
        if (processedShaders.find(shader) == processedShaders.end()) {
            shader->use();
            setLightingUniforms(*shader);
            updateDynamicLights(*shader);
            shader->setUniform("LightSpaceMatrix", lightSpaceMatrix);
            shader->setUniform("ShadowMap", 1); // Assuming shadow map is on texture unit 1
            processedShaders.insert(shader);
        }
    }

    // Render all objects to the HDR buffer
    for (auto& object : scene) {
        object->render(camera);
    }

    // 3. End rendering to HDR buffer, apply bloom, and render to the default framebuffer
    postProcessor->EndRender();

    // After this, the bloom effect has been applied. The final image is displayed to the window.
    //glfwSwapBuffers(window);
    //glfwPollEvents();
}

void ParticleWindow::addCarLights(ppgso::Shader& shader, const glm::vec3& carPosition) {
    // Define offsets for car lights (front-left and front-right)
    glm::vec3 frontLeftLight = carPosition + glm::vec3(-0.5f, 0.5f, 1.0f); // Adjust as needed
    glm::vec3 frontRightLight = carPosition + glm::vec3(0.5f, 0.5f, 1.0f);

    // Set light properties for the front-left light
    shader.setUniform("carLightLeft.position", frontLeftLight);
    shader.setUniform("carLightLeft.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setUniform("carLightLeft.diffuse", glm::vec3(1.0f, 1.0f, 0.8f));
    shader.setUniform("carLightLeft.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("carLightLeft.constant", 1.0f);
    shader.setUniform("carLightLeft.linear", 0.09f);
    shader.setUniform("carLightLeft.quadratic", 0.032f);

    // Set light properties for the front-right light
    shader.setUniform("carLightRight.position", frontRightLight);
    shader.setUniform("carLightRight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setUniform("carLightRight.diffuse", glm::vec3(1.0f, 1.0f, 0.8f));
    shader.setUniform("carLightRight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform("carLightRight.constant", 1.0f);
    shader.setUniform("carLightRight.linear", 0.09f);
    shader.setUniform("carLightRight.quadratic", 0.032f);
}


void ParticleWindow::renderSun(const glm::vec3& lightPos) {
    glPointSize(10.0f); // Adjust the size of the point
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.5f); // Yellowish color
    glVertex3f(lightPos.x, lightPos.y, lightPos.z);
    glEnd();
}

