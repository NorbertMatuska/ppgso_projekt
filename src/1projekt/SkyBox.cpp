#include "Skybox.h"
#include "camera.h"
#include <shaders/skybox_vert_glsl.h>
#include <shaders/skybox_frag_glsl.h>
#include <iostream>

std::unique_ptr<ppgso::Shader> Skybox::shader;
std::unique_ptr<ppgso::Mesh> Skybox::mesh;

Skybox::Skybox(const std::vector<std::string>& faces) {
    if (!shader) shader = std::make_unique<ppgso::Shader>(skybox_vert_glsl, skybox_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");

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
            std::vector<unsigned char> rawData;
            rawData.reserve(image.width * image.height * 3);

            for (int y = 0; y < image.height; ++y) {
                for (int x = 0; x < image.width; ++x) {
                    auto pixel = image.getPixel(x, y);
                    rawData.push_back(pixel.r);
                    rawData.push_back(pixel.g);
                    rawData.push_back(pixel.b);
                }
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                         image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData.data());
        } else {
            std::cerr << "Failed to load cubemap texture at: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

ppgso::Shader* Skybox::getShader() const {
    return shader.get();
}

void Skybox::renderDepth(ppgso::Shader& depthShader) {
    depthShader.use();
    mesh->render();
}


bool Skybox::update(float dTime, Scene &scene) {
    return true;
}

void Skybox::render(const Camera& camera) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    shader->use();

    glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.viewMatrix));
    shader->setUniform("ViewMatrix", viewMatrix);
    shader->setUniform("ProjectionMatrix", camera.projectionMatrix);

    shader->setUniform("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader->setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setUniform("material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
    shader->setUniform("Skybox", 0);

    mesh->render();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
