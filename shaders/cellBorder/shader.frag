#version 330

out vec4 fragColor;

in vec2 fTexCoord;
uniform vec2 mouseCoords;
uniform sampler2D colorScheme;
uniform sampler2D evalField;
uniform int width;
uniform int height;

bool checkMouseCellBorder(vec2 texCoord) {
	float stepX = 1.0f / width;
	float stepY = 1.0f / height;

	float testX = 3.0f * stepX;
	float testY = 3.0f * stepY;

	vec2 rg = texture(evalField, texCoord).rg;	
	vec2 mouseVal = texture(evalField, mouseCoords).rg;
	vec2 c0 = abs(mouseVal - rg);

	if (c0.x < testX && c0.y < testY) {
		//same cell

		if (texture(evalField, texCoord).b > 0.0) {
			vec2 tol = vec2(stepX, stepY);
			vec2 c1 = abs(texture(evalField, texCoord + vec2(stepX, 0.0f)).rg - rg);
			vec2 c2 = abs(texture(evalField, texCoord + vec2(stepX, stepY)).rg - rg);
			vec2 c3 = abs(texture(evalField, texCoord + vec2(0.0f, stepY)).rg - rg);
			vec2 c4 = abs(texture(evalField, texCoord + vec2(-stepX, stepY)).rg - rg);
			vec2 c5 = abs(texture(evalField, texCoord + vec2(-stepX, 0.0f)).rg - rg);
			vec2 c6 = abs(texture(evalField, texCoord + vec2(-stepX, -stepY)).rg - rg);
			vec2 c7 = abs(texture(evalField, texCoord + vec2(0.0f, -stepY)).rg - rg);
			vec2 c8 = abs(texture(evalField, texCoord + vec2(stepX, -stepY)).rg -rg);

			return (!(c1.x < testX && c1.y < testY &&
					  c2.x < testX && c2.y < testY &&
					  c3.x < testX && c3.y < testY &&
					  c4.x < testX && c4.y < testY &&
					  c5.x < testX && c5.y < testY &&
					  c6.x < testX && c6.y < testY &&
					  c7.x < testX && c7.y < testY &&
					  c8.x < testX && c8.y < testY));
		}
	}

	//else
	return false;
}

void main(void)
{
	if (checkMouseCellBorder(fTexCoord)) {
		fragColor = vec4(texture(colorScheme, vec2(0.5f, 0.5f)).rgb, 0.3f);
	} else {
		fragColor = vec4(0.0f,0.0f,0.0f,0.0f);
	}
}