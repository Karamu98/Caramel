uniform vec4 matAmbient = vec4(1, 0, 0, 1);
uniform vec4 matDiffuse = vec4(1, 0, 0, 1);
uniform vec4 matSpecular = vec4(1, 0, 0, 1);

uniform vec4 envAmbient = vec4(1, 0, 0, 1);
uniform vec4 envDiffuse = vec4(1, 0, 0, 1);
uniform vec4 envSpecular = vec4(1, 0, 0, 1);

in vec4 surfaceNormal;
in vec4 surfacePosition;

uniform vec3 camPos
uniform vec3 lightDirection;
uniform float specularPower;

void main()
{
	vec3 ambient = matAmbient * envAmbient; // Ambient Light

	float NdL = max(0.0f, dot(surfaceNormal, -lightDirection)); // Lambert term
	vec4 dif = matDiffuse * envDiffuse * NdL; // Diffuse light for one light

	float reflected = reflect(lightDirection, surfaceNormal);
	vec3 cameraDir = normalize(camPos - surfacePosition); // Surface to eye direction

	float specTerm = pow(min(0.0f, dot(reflected, cameraDir)), specularPower); // Specular term
	vec3 specularPass = matSpecular * envSpecular * specTerm; // Specular light for one light

	glFragColor = vec4(ambient + dif + specularPass, 1);

}


