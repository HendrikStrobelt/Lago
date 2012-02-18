#version 330

out vec4 fragColor;

flat in vec3 fPosWeight;

void main(void)
{
	fragColor = vec4(fPosWeight, 1.0f);
}