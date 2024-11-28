#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 TexCoords;

void main() {
    TexCoords = position;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(position, 1.0);
}
