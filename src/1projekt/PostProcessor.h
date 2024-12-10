#pragma once

#include <GL/glew.h>
#include <memory>
#include "shader.h"

class PostProcessor {
public:
    PostProcessor(unsigned int width, unsigned int height);
    ~PostProcessor();

    void BeginRender();
    void EndRender();

    void setBloomEnabled(bool enabled) { bloomEnabled = enabled; }

private:
    unsigned int width, height;

    unsigned int hdrFBO;
    unsigned int colorBuffers[2];
    unsigned int rboDepth;

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];

    std::unique_ptr<ppgso::Shader> brightExtractShader;
    std::unique_ptr<ppgso::Shader> blurShader;
    std::unique_ptr<ppgso::Shader> finalShader;

    bool bloomEnabled = true;

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    void applyBloom();
    void renderQuad();
};
