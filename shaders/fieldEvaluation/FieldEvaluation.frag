#version 330

uniform int width;
uniform int height;
uniform sampler2D screenTexture;

in vec2 fTex;
out vec4 fragColor;

void main(void) 
{

	float stepX = 1.0f / width;
	float stepY = 1.0f / height;

	bool newPos = true;
	float testVal;
	float currentVal;

	vec2 texPos;
	vec2 newTexPos;


	////START
	texPos = fTex;
	newTexPos = fTex;

	while (newPos) {
		newPos = false;
		currentVal = texture(screenTexture, texPos).b;

		testVal = texture(screenTexture, texPos + vec2(0.0f, stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(0.0f, stepY);
		}
		testVal = texture(screenTexture, texPos + vec2(stepX, stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(stepX, stepY);
		}
		testVal = texture(screenTexture, texPos + vec2(stepX, 0.0f)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(stepX, 0.0f);
		}
		testVal = texture(screenTexture, texPos + vec2(stepX, -stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(stepX, -stepY);
		}
		testVal = texture(screenTexture, texPos + vec2(0.0f , -stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(0.0f, -stepY);
		}
		testVal = texture(screenTexture, texPos + vec2(-stepX , -stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(-stepX, -stepY);
		}
		testVal = texture(screenTexture, texPos + vec2(-stepX, 0.0f)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(-stepX, 0.0f);
		}
		testVal = texture(screenTexture, texPos + vec2(-stepX, stepY)).b;
		if (testVal > currentVal) {
			currentVal = testVal;
			newPos = true;
			newTexPos = texPos + vec2(-stepX, stepY);
		}
			texPos = newTexPos;
	}

	vec2 screen = texPos * 2.0 - 1.0;

	fragColor = vec4(screen.x, screen.y, currentVal, 1.0f);
}
