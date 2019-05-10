#version 400

void main()
{
    gl_FragDepth = gl_FragCoord.z;
}
