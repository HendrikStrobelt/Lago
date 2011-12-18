#version 330

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D colorScheme;
uniform bool borderRun;
uniform bool adaptiveScaleBars;

uniform bool linearMode;
uniform vec4 pointsX;
uniform vec4 pointsY;
uniform float exponent;


float scale(float normedVal) {
	if (linearMode) {
		//linear mode
		
		if (normedVal == 1.0f) {
			//avoid 0 division
			return 1.0f;
		} else {
			if (normedVal >= pointsX.a) {
				return mix(pointsY.a, 1.0, ((normedVal - pointsX.a) / (1.0f - pointsX.a)));
			} else 
			if (normedVal >= pointsX.b) {
				return mix(pointsY.b, pointsY.a, ((normedVal - pointsX.b) / (pointsX.a - pointsX.b)));
			} else 
			if (normedVal >= pointsX.g) {
				return mix(pointsY.g, pointsY.b, ((normedVal - pointsX.g) / (pointsX.b - pointsX.g)));
			} else 
			if (normedVal >= pointsX.r) {
				return mix(pointsY.r, pointsY.g, ((normedVal - pointsX.r) / (pointsX.g - pointsX.r)));
			} else {
				return mix(0.0f, pointsY.r, (normedVal / pointsX.r));
			}
		}
	} else {
		//exponential mode
		return pow(normedVal, exponent); 	
	}
}

void main(void)
{
	if (borderRun) { 
		fragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	} else {
		if (adaptiveScaleBars) {		
			float s = min(1.0f, scale(1.0f-vTexCoord.x));
			fragColor = texture(colorScheme, vec2(s, 0.5f));
		} else {
			fragColor = texture(colorScheme, vec2(1.0f-vTexCoord.x, 0.5f));
		}
	}
}