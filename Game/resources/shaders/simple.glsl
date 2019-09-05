#type vertex
#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aUV;

uniform mat4 camProjView;
uniform mat4 objMatrix;

out Vertex
{
	vec4 pos;
	vec3 normal;
	vec2 uv;
}vert;


void main()
{
	vert.pos = objMatrix * vec4(aPos, 1.0);
	vert.normal = (objMatrix * vec4(aNorm, 1.0)).xyz;
	vert.uv = aUV;
	gl_Position = camProjView * vert.pos;
}

#type fragment
#version 330

struct Light
{
	vec3 pos;
	vec3 colour;
};

struct Material
{
	sampler2D texture;
	sampler2D spec;
	sampler2D emission;
	float emissionBrightness;
	float shininess;
};


uniform Material gMaterial;
uniform Light gLight;
uniform vec3 gCamPos;
uniform float gGamma;


out vec4 FragColour;

in Vertex
{
	vec4 pos;
	vec3 normal;
	vec2 uv;
}vert;

void main()
{
	// Ambient
	vec3 ambient = 0.05 * gLight.colour;

	// Diffuse
	vec3 lightDir = normalize(gLight.pos - vert.pos.xyz);
	vec3 normal = normalize(vert.normal);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * gLight.colour;

	// Specular
	vec3 viewDir = normalize(gCamPos - vert.pos.xyz);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), gMaterial.shininess);
	vec3 specular = gLight.colour * spec * texture(gMaterial.spec, vert.uv).rgb;

	// Emissive
	vec3 emission = texture(gMaterial.emission, vert.uv).rgb * gMaterial.emissionBrightness;

	// Grab unclamped final colour
	vec3 hdr = texture(gMaterial.texture, vert.uv).rgb * (ambient + diffuse + specular + emission);

	/// Reinhard tone mapping
	vec3 mapped = hdr / (hdr + vec3(1.0));

	// Gamma correction
	FragColour = vec4(pow(mapped, vec3(1.0 / gGamma)), 1.0);
}