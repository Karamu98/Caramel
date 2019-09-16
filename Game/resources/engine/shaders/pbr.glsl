#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec2 UV;
out vec3 worldPos;
out vec3 normal;

uniform mat4 objMatrix;
uniform mat4 camProjView;

void main()
{
    UV = aUV;
    worldPos = vec3(objMatrix * vec4(aPos, 1.0));
    normal = mat3(objMatrix) * aNorm;

    gl_Position =  camProjView * vec4(worldPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColour;
in vec2 UV;
in vec3 worldPos;
in vec3 normal;


struct Material
{
	sampler2D albedo;
	sampler2D normal;
	sampler2D metallic;
	sampler2D roughness;
	sampler2D ao;
};

struct Light
{
	vec3 pos;
	vec3 colour;

    float linear;
    float quadratic;
};

uniform Material gMaterial;
uniform Light gLight;

uniform vec3 gCamPos;
uniform float gGamma;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(gMaterial.normal, UV).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(worldPos);
    vec3 Q2  = dFdy(worldPos);
    vec2 st1 = dFdx(UV);
    vec2 st2 = dFdy(UV);

    vec3 N   = normalize(normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// BRDF -----------------------------
// (D) Trowbridge-Reitz GGX
float Distribution(vec3 a_normal, vec3 a_halfway, float roughness);

// (G) Smith's Geometry function
float SchlickGGX(float a_normalDotView, float a_roughness);
float Geometry(vec3 a_normal, vec3 a_viewDir, vec3 a_lightDir, float a_roughness);

// (F) Schlick
vec3 Fresnel(float a_cosTheta, vec3 a_baseReflectance);
//      -----------------------------


void main()
{		
    vec3 albedo     = pow(texture(gMaterial.albedo, UV).rgb, vec3(2.2));
	// TODO: pack these three into a single texture
    float metallic  = texture(gMaterial.metallic, UV).r;
    float roughness = texture(gMaterial.roughness, UV).r;
    float ao        = texture(gMaterial.ao, UV).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(gCamPos - worldPos);

    // Pre-calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation
    vec3 Lo = vec3(0.0);

	{
        // Calculate per-light radiance
        vec3 L = normalize(gLight.pos - worldPos); // Light dir
        vec3 H = normalize(V + L); // Halfway dir
        float distance = length(gLight.pos - worldPos); // Light dist from fragment
        float attenuation = 1.0 / (1.0 + gLight.linear * distance + gLight.quadratic * (distance * distance));
        vec3 radiance = gLight.colour * attenuation;

        // Cook-Torrance BRDF
        float D = Distribution(N, H, roughness);   
        float G = Geometry(N, V, L, roughness);      
        vec3 F  = Fresnel(max(dot(H, V), 0.0), F0);

        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = (D * G * F) / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}


    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 mapped = ambient + Lo;

    // HDR Reinhard tone mapping
    mapped = mapped / (mapped + vec3(1.0));
    // Gamma correction
	FragColour = vec4(pow(mapped, vec3(1.0 / gGamma)), 1.0);
}

float Distribution(vec3 a_normal, vec3 a_halfway, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(a_normal, a_halfway), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float SchlickGGX(float a_normalDotView, float a_roughness)
{
    float r = (a_roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = a_normalDotView;
    float denom = a_normalDotView * (1.0 - k) + k;

    return nom / denom;
}

float Geometry(vec3 a_normal, vec3 a_viewDir, vec3 a_lightDir, float a_roughness)
{
    float normalDotView = max(dot(a_normal, a_viewDir), 0.0);
    float normalDotLight = max(dot(a_normal, a_lightDir), 0.0);
    float ggx1 = SchlickGGX(normalDotLight, a_roughness);
    float ggx2 = SchlickGGX(normalDotView, a_roughness);

    return ggx1 * ggx2;
}


vec3 Fresnel(float a_cosTheta, vec3 a_baseReflectance)
{
    return a_baseReflectance + (1.0 - a_baseReflectance) * pow(1.0 - a_cosTheta, 5.0);
}