#version 330 core
out vec4 FragColor;

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

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 2
#define NR_SPOT_LIGHTS 4

in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight directionalLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

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
    for(int i = 0; i< NR_DIR_LIGHTS; i++)
    {
      result += CalcDirLight(directionalLights[i], FragNorm, viewDir, Specular);
    }
/*
    // Point lighting
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
      result += CalcPointLight(pointLights[i], FragNorm, FragPos, viewDir);
    }

    // Spot lighting
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
      result += CalcSpotLight(spotLights[i], FragNorm, FragPos, viewDir);
    }

    */
    FragColor = vec4(Diffuse * result + 0.1, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float specularVal)
{
  // Ambient calculation
  float NdotL = max( 0, dot( normal, -light.direction));
  vec3 diffuse = light.diffuse * NdotL;

  //specular light value
  vec3 R = reflect(light.direction, normal);
  vec3 E = viewDir;
  float specTerm = pow(min(0.0, dot(R,E)), 32.0);
  vec3 spec = light.specular * specTerm * specularVal;


  return diffuse + spec;

}
