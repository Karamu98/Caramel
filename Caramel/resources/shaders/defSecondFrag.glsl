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
vec3 Normal;
vec3 Diffuse;
float Specular;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{

    // Getting the data from the first pass
    FragPos = texture(gPosition, TexCoords).rgb;
    Normal = texture(gNormal, TexCoords).rgb;
    Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    Specular = texture(gAlbedoSpec, TexCoords).a;

    // properties
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    for(int i = 0; i< NR_DIR_LIGHTS; i++)
    {
      result += CalcDirLight(directionalLights[i], Normal, viewDir);
    }

/*
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
      result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
    }
    // phase 3: spot light
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
      result += CalcSpotLight(spotLights[i], Normal, FragPos, viewDir);
    }
    FragColor = vec4(result, 1.0);
    */
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  // Ambient calculation
  vec3 ambient = light.ambient * Diffuse;

  // Diffuse calculation
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(Normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * Diffuse;

  // Specular Calculation
  vec3 reflectDir = reflect(-lightDir, Normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
  vec3 specular = light.specular * spec * Specular;

  return (ambient + diffuse + specular);

}
