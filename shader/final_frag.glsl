#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main() {
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (bloom)
        hdrColor += bloomColor; // Add bloom

    // Simple Reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    FragColor = vec4(mapped, 1.0);
}