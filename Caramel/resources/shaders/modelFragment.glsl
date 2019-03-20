#version 150

in Vertex
{
  vec4 vPosition;
  vec4 vNormal;
  vec2 vUV;
  vec4 vTans;
  vec4 vBiTans;
  vec3 HalfDir;
  vec3 LightDir;
}vertex;

out vec4 FragColor;

uniform sampler2D material.texture_diffuse1;
uniform sampler2D material.texture_specular1;
uniform sampler2D material.texture_normal1;
uniform sampler2D material.texture_height1;

 struct Light{
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
 };

 const Light sceneLight = Light(vec3( 0.25, 0.25,0.25), vec3(1.0,1.0,1.0), vec3(1.0,1.0,1.0));
 const float specularTerm = 32.0;

void main()
{

	vec3 albedo = vec3( 1.0, 0.4, 0.4 );

	//Phong ambient color
	vec3 ambient = sceneLight.ambient;
	//Phong Diffuse
	vec3 diffuse = max(0,dot(vNormal.xyz, vertex.LightDir))* sceneLight.diffuse;

	//Calculate Specular Component
	vec3 specular = pow(max(0,dot(vertex.HalfDir, vertex.Normal.xyz)),specularTerm)*sceneLight.specular;

	vec3 linearColour = albedo * (ambient + diffuse + specular);
	//gamma correction
	vec3 gammaCorrected = pow(linearColour, vec3(1.0/2.2));
	FragColor = vec4(gammaCorrected, 1.0f);
}
