#pragma once

#include <GL/glew.h>
#include <memory>
#include "shader.h"

class PostProcessor {
public:
    // Constructor: set up the HDR framebuffer, ping-pong FBOs for bloom
    PostProcessor(unsigned int width, unsigned int height);
    ~PostProcessor();

    // Start rendering into HDR framebuffer
    void BeginRender();
    // Finish rendering: apply bloom and render to default framebuffer
    void EndRender();

    // Enable/disable bloom
    void setBloomEnabled(bool enabled) { bloomEnabled = enabled; }

private:
    unsigned int width, height;

    // HDR framebuffer and textures
    unsigned int hdrFBO;
    unsigned int colorBuffers[2];
    unsigned int rboDepth;

    // Ping-pong framebuffers for blurring the bright parts
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    // Shaders
    std::unique_ptr<ppgso::Shader> brightExtractShader;
    std::unique_ptr<ppgso::Shader> blurShader;
    std::unique_ptr<ppgso::Shader> finalShader;

    // State
    bool bloomEnabled = true;

    // Fullscreen quad setup
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    // Internal methods
    void applyBloom();
    void renderQuad();
};
