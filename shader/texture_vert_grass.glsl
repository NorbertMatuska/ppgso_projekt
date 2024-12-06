#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float TilingFactor; // Tiling factor for texture scaling

out vec2 TexCoord;

void main() {
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);
    TexCoord = texCoord * TilingFactor; // Scale UVs by tiling factor
}