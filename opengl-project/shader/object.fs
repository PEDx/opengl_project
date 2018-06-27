#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D ourTexture;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;


void main()
{
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * material.diffuse * light.diffuse;


  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = material.specular * spec * light.specular;

  vec3 ambient = material.ambient * light.ambient;

  // FragColor = mix(vec4(lightColor * objectColor , 1.0), texture(ourTexture, TexCoord), 0.1);
  FragColor = vec4(ambient + diffuse + specular , 1.0);
}
