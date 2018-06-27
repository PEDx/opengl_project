#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
  FragColor = mix(vec4(lightColor * objectColor, 1.0), texture(ourTexture, TexCoord), 0.5);
}
