#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;
out vec3 FragPos;
out vec3 NormalDir;

void main() {
    texCoord = TexCoord;
    FragPos = vec3(ModelMatrix * vec4(Position, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(ModelMatrix)));
    NormalDir = normalize(normalMatrix * Normal);
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}
