#version 330

uniform int width;
uniform int height;
uniform int seedWidth;
uniform int seedHeight;
uniform int pixelQuad;
uniform sampler2D screenTexture;
uniform sampler2D seedTexture;

in vec2 fTex;
out vec4 fragColor;

void main(void) 
{

	float stepX = 1.0f / width;
	float stepY = 1.0f / height;

	float stepSeedX = 1.0f / seedWidth;
	float stepSeedY = 1.0f / seedHeight;


	bool newPos = true;
	float testVal;
	float currentVal = 0;

	vec2 texPos;
	vec2 newTexPos;


	////START
	texPos = fTex;
	newTexPos = fTex;

	vec2 seedTestCoord = texPos;
	seedTestCoord = seedTestCoord - vec2((stepX / 2.0), (stepY / 2.0)); //not center but pixel start
	seedTestCoord = seedTestCoord * vec2(width, height); // in pixel 0..width-1
	seedTestCoord = seedTestCoord + vec2(pixelQuad, pixelQuad); //in seedTex pixel
	seedTestCoord = seedTestCoord / vec2(seedWidth, seedHeight); 
	seedTestCoord = seedTestCoord + vec2((stepSeedX / 2.0), (stepSeedY / 2.0));
		
	

	if (texture(seedTexture, seedTestCoord).b > 0.0) {
		currentVal = texture(screenTexture, texPos).b;
	
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

	}

	vec2 screen = texPos * 2.0 - 1.0;

	fragColor = vec4(screen.x, screen.y, currentVal, 1.0f);
}
