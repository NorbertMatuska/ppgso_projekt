#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

uniform float grainIntensity;
uniform float grainScale;
uniform float grainTime;

uniform float chromaticAberration;

// pseudo-random for grain
float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (bloom)
        hdrColor += bloomColor; // add bloom

    // tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    // grain
    float noise = rand(TexCoords * grainScale + grainTime) * 2.0 - 1.0;
    mapped += noise * grainIntensity;

    // distance to the top-right corner
    vec2 corner = vec2(1.0, 1.0); // Assuming normalized coordinates
    float dist = length(TexCoords - corner);

    // curve effect for CE
    float falloff = 1.0 - smoothstep(0.2, 0.6, dist); // Adjust range (0.2, 0.6) as needed
    float caStrength = chromaticAberration * falloff;

    vec2 greenOffset = vec2(caStrength, 0.0);
    vec2 blueOffset  = vec2(-caStrength, 0.0);

    // chromatic aberration
    vec3 colorR = texture(scene, TexCoords).rgb; // Red channel stays at TexCoords
    vec3 colorG = texture(scene, TexCoords + greenOffset).rgb;
    vec3 colorB = texture(scene, TexCoords + blueOffset).rgb;

    // combine channels
    vec3 finalColor = vec3(colorR.r, colorG.g, colorB.b);

    // add grain noise
    finalColor += noise * grainIntensity;
    finalColor = clamp(finalColor, 0.0, 1.0);

    FragColor = vec4(finalColor, 1.0);
}
