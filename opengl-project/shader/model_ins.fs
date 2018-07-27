#version 330 core

out vec4 FragColor;

in VS_OUT
{
  vec2 texCoords;
  vec3 normal;
  vec3 fragPos;
  vec4 fragPosLightSpace;
}
fs_in;

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
uniform sampler2D shadowMap;

uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform vec3 viewPos;

const int shininess = 64;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
  vec3 norm = normalize(fs_in.normal);
  vec3 viewDir = normalize(viewPos - fs_in.fragPos);

  vec3 result = CalcDirLight(dirLight, norm, viewDir);

  for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
    result += CalcPointLight(pointLights[i], norm, fs_in.fragPos, viewDir);

  FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 color = texture(texture_diffuse1, fs_in.texCoords).rgb;
  vec3 lightDir = normalize(-light.direction);

  vec3 ambient = light.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = light.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

  float shadow = ShadowCalculation(fs_in.fragPosLightSpace);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  return lighting;
  //  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 color = texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 lightDir = normalize(light.position - fragPos);

  vec3 ambient = light.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = light.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

  // 衰减
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));

  float shadow = ShadowCalculation(fs_in.fragPosLightSpace);
  return (ambient + (1.0 - shadow) * (diffuse + specular)) * color * attenuation;
  // vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  // return lighting;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // check whether current frag pos is in shadow
  float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

  return shadow;
}