#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


#define POINT_LIGHTS_COUNT 4

struct DirLight
{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight
{
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform vec3 viewPos;

const int  shininess = 64;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result = CalcDirLight(dirLight, norm, viewDir);

  for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

  FragColor = vec4(result, 1.0);;
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.direction);

  vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;

  return ambient + diffuse + specular;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - FragPos);

  vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;

  // 衰减
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));

  return (ambient + diffuse + specular) * attenuation;
}
