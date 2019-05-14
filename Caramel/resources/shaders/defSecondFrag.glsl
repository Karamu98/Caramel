#version 400
layout (location = 0) out vec4 FragColour;
layout (location = 1) out vec4 BloomColour;

struct DirLight
{
    vec3 direction;

    vec3 diffuse;
    vec3 specular;

    mat4 projViewMatrix;
    vec2 atlasIndex;
};

struct PointLight
{
    vec3 position;

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

    float linear;
    float quadratic;

    vec3 diffuse;
    vec3 specular;

    mat4 projViewMatrix;
    vec2 atlasIndex;
};

#define MAX_POINT_LIGHTS 64
#define MAX_DIR_LIGHTS 1
#define MAX_SPOT_LIGHTS 16

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
uniform int shadowTexSize;
uniform int shadowWH;

uniform float hdrGamma;
uniform float bloomMin;

vec3 FragPos;
vec3 FragNorm;
vec4 Diffuse;
float Specular;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specVal);
float CalcShadow(mat4 a_lightSpaceMatrix, vec2 a_shadowIndex);
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


    /// Getting the final colour unclamped
    vec3 hdrColour = Diffuse.rgb * result;

    BloomColour = vec4(max(vec3(0.0), hdrColour - bloomMin), 1.0);

    /// Reinhard tone mapping
    vec3 mapped = hdrColour / (hdrColour + vec3(1.0));

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / hdrGamma));

    FragColour = vec4(mapped, 1.0);
}

float CalcShadow(mat4 a_lightProjView, vec2 a_shadowIndex)
{
  // Get the fragment in the lights projection view matrix
  vec4 fragPosLightSpace = a_lightProjView * vec4(FragPos.xyz, 1.0);

  // Perspective division and convert to 0 to 1 range
  vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * vec3(0.5) + vec3(0.5);

  // Get the depth of the current fragment from the lights perspective
  float currentDepth = projCoords.z;

  //vec2 shadCoords = (projCoords.xy / shadowWH); // TODO:: Shadow Atlas
  //shadCoords = shadCoords * a_shadowIndex;

  // Check whether the current fragpos is shadowed
  float shadow = 0.0;

  vec2 texelSize = 1.0 / textureSize(gShadow, 0);// / shadowWH;
  for(int x = -3; x <= 3; ++x)
  {
      for(int y = -3; y <= 3; ++y)
      {
          float pcfDepth = texture(gShadow, projCoords.xy + vec2(x, y) * texelSize).r;
          shadow += step(pcfDepth, currentDepth);
      }
  }
  shadow /= 49.0;

  if(projCoords.z > 1.0)
  {
    return 0.0;
  }

  return shadow;
  //return step(shadowMapDepth, currentDepth);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specVal)
{
  vec3 lightDir = normalize(-light.direction);

  // Diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, lightDir), 0.0), specVal);

  // Combine results
  vec3 ambient = light.diffuse * 0.05f;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shad = CalcShadow(light.projViewMatrix, light.atlasIndex);

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
  float attenuation = 1.0 / (1.0 + light.linear * fDistance + light.quadratic * (fDistance * fDistance));

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
  float attenuation = 1.0 / (1 + light.linear * fDistance + light.quadratic * (fDistance * fDistance));

  // Spotlight intensity, is the fragment in range of the spot light? Change its intensity based on that
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // Combine results
  vec3 ambient = light.diffuse * 0.05f;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shad = CalcShadow(light.projViewMatrix, light.atlasIndex);

  // Attenuate
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;

  return (ambient + (1.0 - shad) * (diffuse + specular));
}
