#include "Skybox.h"
#include "camera.h" // Include Camera definition
#include <shaders/skybox_vert_glsl.h>
#include <shaders/skybox_frag_glsl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Static resources
std::unique_ptr<ppgso::Shader> Skybox::shader;
std::unique_ptr<ppgso::Mesh> Skybox::mesh;

Skybox::Skybox(const std::vector<std::string>& faces) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(skybox_vert_glsl, skybox_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");

    // Load the cubemap textures
    loadCubemap(faces);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &cubemapTextureID);
}

void Skybox::loadCubemap(const std::vector<std::string>& faces) {
    glGenTextures(1, &cubemapTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        auto image = ppgso::image::loadBMP(faces[i]);

        if (image.width > 0 && image.height > 0) {
            // Convert image pixel data to a raw byte array
            std::vector<unsigned char> rawData;
            rawData.reserve(image.width * image.height * 3); // Reserve space for RGB values

            for (int y = 0; y < image.height; ++y) {
                for (int x = 0; x < image.width; ++x) {
                    auto pixel = image.getPixel(x, y);
                    rawData.push_back(pixel.r); // Red channel
                    rawData.push_back(pixel.g); // Green channel
                    rawData.push_back(pixel.b); // Blue channel
                }
            }

            // Upload the raw pixel data to the cubemap texture
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                         image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData.data());
        } else {
            std::cerr << "Failed to load cubemap texture at: " << faces[i] << std::endl;
        }
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

bool Skybox::update(float dTime, Scene &scene) {
    return true; // Skybox does not need updates
}

void Skybox::render(const Camera& camera) {
    // Set depth function to GL_LEQUAL to render the skybox behind everything
    glDepthFunc(GL_LEQUAL);

    // Disable depth writes (the skybox should not occlude other objects)
    glDepthMask(GL_FALSE);

    shader->use();

    // Remove translation from the view matrix (keep the skybox centered)
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.viewMatrix));
    shader->setUniform("ViewMatrix", viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    // Bind the cubemap texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
    shader->setUniform("Skybox", 0);

    // Render the cube
    mesh->render();

    // Restore depth mask and depth function
    glDepthMask(GL_TRUE); // Enable depth writes
    glDepthFunc(GL_LESS); // Reset depth function
}
