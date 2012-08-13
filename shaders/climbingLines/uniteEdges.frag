#version 330

out vec4 fragColor;

in vec2 fTexCoord;

uniform sampler2D angleTex;
uniform sampler2D assembleTex;

void main(void)
{

	vec4 colAngle = texture(angleTex, fTexCoord);
	float max;

	max = texture(assembleTex, fTexCoord).g;

	if (colAngle.r > max) {
		max = colAngle.r;
	}
	if (colAngle.g > max) {
		max = colAngle.g;
	}
	if (colAngle.b > max) {
		max = colAngle.b;
	}
	if (colAngle.a > max) {
		max = colAngle.a;
	}

	fragColor = vec4(0.0f, max, 0.0f, 1.0f);
}