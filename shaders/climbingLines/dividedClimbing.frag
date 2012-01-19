#version 330

out vec4 fragColor;

flat in float fWeight;
flat in int fPos;

void main(void)
{
	if (fPos == 0) {
		fragColor = vec4(fWeight, 0.0f, 0.0f, 0.0f);   
	} else 
	if (fPos == 1) {
		fragColor = vec4(0.0f, fWeight, 0.0f, 0.0f);   
	} else 
	if (fPos == 2) {
		fragColor = vec4(0.0f, 0.0f, fWeight, 0.0f);   
	} else {
		fragColor = vec4(0.0f, 0.0f, 0.0f, fWeight);   
	}
}