#version 330 core
out vec4 FragColour;

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;


uniform vec3 u_ViewPos;

// TODO: Deferred rendering
uniform vec3 u_LightPos;
uniform vec3 u_LightColour;

// Fill from assimp
uniform vec3 u_DiffuseColour;
uniform vec3 u_AmbientColour;
uniform vec3 u_SpecularColour;
uniform sampler2D u_DiffuseMap0;
uniform sampler2D u_SpecularMap0;

void main()
{
    // Texture samples
    vec3 diffuseSample = texture(u_DiffuseMap0, UV).rgb;
    vec3 specularSample = texture(u_SpecularMap0, UV).rgb;

    // DEBUG
    vec3 lightPos = u_ViewPos;
    vec3 lightColour = vec3(1);

    // Ambient
    vec3 ambient = u_AmbientColour * lightColour;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // Specular
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = u_SpecularColour * spec * lightColour * specularSample;  
    
    vec3 result = (ambient + diffuse + specular) * u_DiffuseColour * diffuseSample;
    FragColour = vec4(result, 1.0);

    //FragColour = vec4(diffuseSample, 1.0);
} 