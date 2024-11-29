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
    texCoord = TexCoord;
    FragPos = vec3(ModelMatrix * vec4(Position, 1.0));
    NormalDir = mat3(transpose(inverse(ModelMatrix))) * Normal;
    FragPosLightSpace = LightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
}
