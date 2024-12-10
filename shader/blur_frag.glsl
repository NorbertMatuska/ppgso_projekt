#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;

void main() {
    // gaussian weights (5 tap)
    float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    ivec2 texSize = textureSize(image, 0);
    float texOffsetX = 1.0 / float(texSize.x);
    float texOffsetY = 1.0 / float(texSize.y);

    vec3 result = texture(image, TexCoords).rgb * weight[0];
    for (int i = 1; i < 5; i++) {
        if (horizontal) {
            result += texture(image, TexCoords + vec2(texOffsetX * float(i), 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(texOffsetX * float(i), 0.0)).rgb * weight[i];
        } else {
            result += texture(image, TexCoords + vec2(0.0, texOffsetY * float(i))).rgb * weight[i];
            result += texture(image, TexCoords - vec2(0.0, texOffsetY * float(i))).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}