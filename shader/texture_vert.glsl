#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 LightSpaceMatrix;

out vec2 texCoord;
out vec3 FragPos;
out vec3 NormalDir;
out vec4 FragPosLightSpace;

void main() {
    vec4 worldPosition = ModelMatrix * vec4(Position, 1.0);
    FragPos = vec3(worldPosition);
    NormalDir = mat3(transpose(inverse(ModelMatrix))) * Normal;
    FragPosLightSpace = LightSpaceMatrix * worldPosition;
    gl_Position = ProjectionMatrix * ViewMatrix * worldPosition;
    texCoord = TexCoord;
}
