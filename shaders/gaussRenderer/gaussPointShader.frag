#version 330

out vec4 fragColor;

flat in float fWeight;

void main(void)
{
	fragColor = vec4(0.0f , 0.0f, fWeight, 1.0f);
}