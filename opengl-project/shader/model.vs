#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

#define POINT_LIGHTS_COUNT 4

out VS_OUT {
    vec2 texCoords;
    vec3 fragPos;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
    vec3 pointLightsPos[POINT_LIGHTS_COUNT];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 pointLightsPos[POINT_LIGHTS_COUNT];
uniform vec3 viewPos;

void main()
{
    mat3 TBN = transpose(mat3(aTangent, aBitangent, aNormal));

    vs_out.texCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.fragPos = vec3(model * vec4(aPos, 1.0));

    vs_out.tangentLightPos = TBN * lightPos;
    vs_out.tangentViewPos  = TBN * viewPos;
    vs_out.tangentFragPos  = TBN * vs_out.fragPos;

    for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
        vs_out.pointLightsPos[i] = vec3(TBN * pointLightsPos[i]);
}