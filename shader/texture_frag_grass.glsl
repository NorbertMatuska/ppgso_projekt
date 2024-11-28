#version 330 core

in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 FragColor;

void main() {
    vec4 textureColor = texture(Texture, TexCoord);
    if (textureColor.a < 0.1)
        discard;
    FragColor = textureColor;
}
