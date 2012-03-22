#version 330

out vec4 fragColor;

in vec2 fTexCoord;
uniform sampler2D evalTexture;
uniform sampler2D screenTexture;
uniform vec2 compareTexCoord;
uniform int width;
uniform int height;

void main(void)
{
/*
	float stepX = 1.0f / width;
	float stepY = 1.0f / height;

	if (fTexCoord.x + stepX > compareTexCoord.x && fTexCoord.x - stepX < compareTexCoord.x &&
	    fTexCoord.y + stepY > compareTexCoord.y && fTexCoord.y - stepY < compareTexCoord.y) {

		//evaluate value
		bool newPos = true;
		float testVal;
		float currentVal = 0;

		vec2 texPos;
		vec2 newTexPos;


		////START
		texPos = fTexCoord;
		newTexPos = fTexCoord;

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

	} else {
		fragColor = texture(evalTexture, fTexCoord);
	}
*/
	fragColor = texture(evalTexture, fTexCoord);
}