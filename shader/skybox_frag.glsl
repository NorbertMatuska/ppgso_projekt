#version 330 core

in vec3 TexCoords;

uniform samplerCube Skybox;

out vec4 FragColor;

void main() {
    FragColor = texture(Skybox, TexCoords);
}
