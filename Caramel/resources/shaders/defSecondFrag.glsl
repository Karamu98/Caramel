#version 330 core
out vec4 FragColor;

struct DirLight
{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 64
#define MAX_DIR_LIGHTS 4
#define MAX_SPOT_LIGHTS 64

in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight directionalLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

vec3 FragPos;
vec3 FragNorm;
vec3 Diffuse;
float Specular;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specVal);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

    // Getting the data from the first pass
    FragPos = texture(gPosition, TexCoords).rgb;
    FragNorm = texture(gNormal, TexCoords).rgb * 2.0 - 1.0;
    vec4 albedoSpec = texture(gAlbedoSpec, TexCoords);
    Diffuse = albedoSpec.rgb;
    Specular = albedoSpec.a;

    // properties
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0,0,0);

    // Directional lighting
    for(int i = 0; i < MAX_DIR_LIGHTS; i++)
    {
      result += CalcDirLight(directionalLights[i], FragNorm, viewDir, Specular);
    }

/*
    // Point lighting
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
      result += CalcPointLight(pointLights[i], FragNorm, FragPos, viewDir);
    }

    // Spot lighting
    for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
      result += CalcSpotLight(spotLights[i], FragNorm, FragPos, viewDir);
    }
    */

    FragColor = vec4(Diffuse * result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specularVal)
{
  // Ambient lighting
  vec3 ambient = 0.05f * Diffuse;

  // Diffuse calculation
  float NdotL = max( 0, dot( normal, -light.direction));
  vec3 diffuse = Diffuse * NdotL;

  // Specular calculation
  vec3 reflectDir = reflect(-light.direction, normal);
  vec3 halfwayDir = normalize(-light.direction + viewDir);
  float specTerm = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0);
  vec3 spec = light.specular * specTerm;


  return ambient + diffuse + spec;

}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  // Ambient lighting
  vec3 ambient = 0.05f * Diffuse;

  // Diffuse calculation
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular calculation
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

  // Attenuation
  float fDistance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * fDistance + light.quadratic * (fDistance * fDistance));

  // Combine results
  vec3 diffuse  = light.diffuse  * diff * Diffuse;
  vec3 specular = light.specular * spec;
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}
