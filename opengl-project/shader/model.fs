#version 330 core

out vec4 FragColor;

#define POINT_LIGHTS_COUNT 4

in VS_OUT
{
  vec2 texCoords;
  vec3 fragPos;
  vec3 tangentLightPos;
  vec3 tangentViewPos;
  vec3 tangentFragPos;
  vec3 pointLightsPos[POINT_LIGHTS_COUNT];
}
fs_in;



struct DirLight
{
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight
{
  // vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform DirLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform bool blinn;

const int shininess = 64;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 position);

void main()
{
  vec3 normal = texture(texture_normal1, fs_in.texCoords).rgb;
    // Transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

  vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);

  vec3 result = CalcDirLight(dirLight, normal, viewDir);

  for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
    result += CalcPointLight(pointLights[i], normal, fs_in.fragPos, viewDir, fs_in.pointLightsPos[i]);

  // apply gamma correction
  // float gamma = 2.2;
  // result = pow(result, vec3(1.0 / gamma));
  FragColor = vec4(result * 2.0, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-fs_in.tangentLightPos);

  vec3 ambient = light.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;

  float spec;
  if (blinn)
  {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(norm, halfwayDir), 0.0), shininess * 2.0);
  }
  else
  {
    vec3 reflectDir = reflect(-lightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  }
  vec3 specular = light.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 position)
{
  vec3 lightDir = normalize(position - fragPos);

  vec3 ambient = light.ambient * texture(texture_diffuse1, fs_in.texCoords).rgb;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, fs_in.texCoords).rgb;

  float spec;
  if (blinn)
  {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(norm, halfwayDir), 0.0), shininess * 2.0);
  }
  else
  {
    vec3 reflectDir = reflect(-lightDir, norm);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  }
  vec3 specular = light.specular * spec * texture(texture_specular1, fs_in.texCoords).rgb;

  // 衰减
  float distance = length(position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));

  return (ambient + diffuse + specular) * attenuation;
}
