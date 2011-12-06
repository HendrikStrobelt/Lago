#version 330

out vec4 fragColor;

in vec2 vTexCoord;
uniform sampler2D inputTexture;
uniform int kernelSize;
uniform int width;
uniform int height;
uniform int parentWidth;
uniform int parentHeight;

void main(void)
{

	vec4 colorMax = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 color;

	float xStart = (vTexCoord.x * width * kernelSize) - (0.5 * (kernelSize-1));
	float yStart = (vTexCoord.y * height * kernelSize) - (0.5 * (kernelSize-1));

	for (int x = 0; x < kernelSize; x++) {
		for (int y = 0; y < kernelSize; y++) {
			

			float s = (xStart + x) / parentWidth;
			float t = (yStart + y) / parentHeight;

			color = texture(inputTexture, vec2(s, t)); 
			if (color.b > colorMax.b) {
				colorMax.b = color.b;
			}
			if (color.r > colorMax.r) {
				colorMax.r = color.r;
			}
			if (color.g > colorMax.g) {
				colorMax.g = color.g;
			}

		}
	}

    fragColor = colorMax;
}