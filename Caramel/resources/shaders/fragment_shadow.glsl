#version 330 core

out float FragDepth;

void main()
{
	FragDepth = gl_FragCoord.z;
}
