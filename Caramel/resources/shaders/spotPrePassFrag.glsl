#version 400

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 100.0;

    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}
