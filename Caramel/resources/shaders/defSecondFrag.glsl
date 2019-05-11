#version 400
out vec4 FragColor;

struct DirLight
{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;

    mat4 lightSpaceMatrix;
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

    //mat4 lightSpaceMatrix;
};

#define MAX_POINT_LIGHTS 64
#define MAX_DIR_LIGHTS 4
#define MAX_SPOT_LIGHTS 64

in vec2 TexCoords;

uniform vec3 viewPos;
uniform int dirLightCount;
uniform int pointLightCount;
uniform int spotLightCount;
uniform DirLight directionalLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpec;
uniform sampler2D gShadow;

vec3 FragPos;
vec3 FragPosLightSpace;
vec3 FragNorm;
vec4 Diffuse;
float Specular;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specVal);
float CalcShadow(mat4 a_lightSpaceMatrix);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float specVal);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float specVal);

void main()
{

    // Getting the data from the first pass
    FragPos = texture(gPosition, TexCoords).rgb;
    FragNorm = normalize(texture(gNormal, TexCoords).rgb);
    vec4 albedoSpec = texture(gAlbedo, TexCoords);
    Diffuse = texture(gAlbedo, TexCoords);
    Specular = texture(gSpec, TexCoords).r * 128;


    // properties
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0,0,0);
    float shadow = 0;

    // Directional lighting
    for(int i = 0; i < dirLightCount; i++)
    {
      result += CalcDirLight(directionalLights[i], FragNorm, viewDir, Specular);
    }

    // Point lighting
    for(int i = 0; i < pointLightCount; i++)
    {
      result += CalcPointLight(pointLights[i], FragNorm, FragPos, viewDir, Specular);
    }

    // Spot lighting
    for(int i = 0; i < spotLightCount; i++)
    {
      result += CalcSpotLight(spotLights[i], FragNorm, FragPos, viewDir, Specular);
    }


    FragColor = vec4(Diffuse.rgb * result, 1.0);
}

float CalcShadow(mat4 a_lightSpaceMatrix)
{
  vec4 fragPosLightSpace = vec4(FragPos.xyz, 1.0) * a_lightSpaceMatrix; // Get the fragment in the lights space
  //vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

  vec3 projCoords = (fragPosLightSpace.xyz * 0.5) + 0.5; // Get the value between 0 and 1

  if(projCoords.z > 1.0)
  {
    return 0.0;
  }

  float shadowMapDepth = texture(gShadow, projCoords.xy).r;

  float currentDepth = projCoords.z;

  return step(currentDepth, shadowMapDepth);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specVal)
{
  vec3 lightDir = normalize(-light.direction);

  // Diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), specVal);

  // Combine results
  vec3 ambient = light.diffuse * 0.05f;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shad = CalcShadow(light.lightSpaceMatrix);

  //return (ambient + diffuse + specular);
  return (ambient + (1.0 - shad) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float specVal)
{
  vec3 lightDir = normalize(light.position - fragPos);

  // Diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), specVal);

  // Attenuation calculation
  float fDistance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * fDistance + light.quadratic * (fDistance * fDistance));

  // Combine results
  vec3 ambient = light.diffuse * 0.05f; // Light diffuse to allow for complete darkness
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  // Attenuate
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float specVal)
{
  vec3 lightDir = normalize(light.position - fragPos);

  // Diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), specVal);

  // Attenuation calculation
  float fDistance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * fDistance + light.quadratic * (fDistance * fDistance));

  // Spotlight intensity, is the fragment in range of the spot light? Change its intensity based on that
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // Combine results
  vec3 ambient = light.diffuse * 0.05f;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shad = 0;//CalcShadow(light.lightSpaceMatrix);

  // Attenuate
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;

  return (ambient + (1.0 - shad) * (diffuse + specular));
}
