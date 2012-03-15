#version 330

out vec4 fragColor;

in vec2 fTexCoord;
uniform float maxValue;
uniform sampler2D colorScheme;
uniform sampler2D lineField;
uniform sampler2D gaussTex;

uniform int width;
uniform int height;
uniform bool antiAlias;

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




vec4 getColor(vec2 texCoordCenter, vec2 texCoord, float stepX , float stepY) {
	float max = 0.0f;


	//Pattern pseudo euklid
	int rad = 4;

	for (int y = -rad; y <= rad; y++) {
		for (int x = -rad; x <= rad; x++) {

			vec2 otherPosCenter = (texCoordCenter + vec2(x*stepX, y*stepY));

			float len = length((texCoord - otherPosCenter) * vec2(width, height));
			float val = scale(texture(lineField, otherPosCenter).g / maxValue);

			if ((x < 0) || (x == 0 && y <= 0)) {
				if ((val * (rad*2+1)) > (len * 2)) {
					if (val > max) {
						max = val;
					}
				}
			} else {
				if ((val * (rad*2+1)) > ((len * 2) - 1)) {
					if (val > max) {
						max = val;
					}
				}
			}
		} 
	} 

	if (max > 0.0f) {
		
		vec4 influence = texture(colorScheme, vec2(max, 0.5f));
		vec3 oldResult = texture(gaussTex, texCoordCenter).rgb;
		
		//with influence mix them
		vec3 col =  influence.rgb;//mix(oldResult, influence.rgb, influence.a);
		
		return vec4(col, 1.0f);
	} else {
		
		//no influence simply return the result so far
		//return vec4(texture(gaussTex, texCoordCenter).rgb, 1.0f);
		return vec4(1,1,1,1);
	}
}



void main(void)
{

	float stepX = 1.0f / width;
	float stepY = 1.0f / height;
	vec4 color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	if (antiAlias) {
		
		int empty = 0;
		vec2 texCoord;
		for (int run = 0; run < 4; run++) {

			switch (run) {
				case 0:
					texCoord = fTexCoord + vec2((stepX * 0.5f), (stepY * 0.5f));
				break;
				case 1:
					texCoord = fTexCoord + vec2((stepX * 0.5f), (stepY * -0.5f));				
				break;
				case 2:
					texCoord = fTexCoord + vec2((stepX * -0.5f), (stepY * -0.5f));				
				break;
				case 3:
					texCoord = fTexCoord + vec2((stepX * -0.5f), (stepY * 0.5f));				
				break;
			}

			color +=  getColor(fTexCoord, texCoord, stepX, stepY);
		}

		color = color / vec4(4,4,4,4);
		color +=  getColor(fTexCoord, fTexCoord, stepX, stepY);
		
		fragColor = color / vec4(2,2,2,2);
	} else {

		fragColor = getColor(fTexCoord, fTexCoord, stepX, stepY);
	}
}


/*
vec3 rgbFrom(float hue, float saturation, float brightness) {

	float r = 0, g = 0, b = 0;

	if (saturation == 0) {
	    r = g = b = brightness;
	} else {
	    float h = (hue - floor(hue)) * 6.0f;
	    float f = h - floor(h);
	    float p = brightness * (1.0f - saturation);
	    float q = brightness * (1.0f - saturation * f);
	    float t = brightness * (1.0f - (saturation * (1.0f - f)));

		int int_h = int(h);

	    switch (int_h) {
			case 0:
				r = brightness;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = brightness;
				b = p;
				break;
			case 2:
				r = p;
				g = brightness;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = brightness;
				break;
			case 4:
				r = t;
				g = p;
				b = brightness;
				break;
			case 5:
				r = brightness;
				g = p;
				b = q;
			break;
	    }
	}

	return vec3(r,g,b);
}



//from getColor
		//float hue = ((1.0f - max) * 90.0f)/ 360.0f;
		//float alpha = (0.66f + max / 3.0f);
		float hue = 0;
		float sat = 0;
		float alpha = (0.66f + max / 3.0f);
		float bright = 1.0f - (0.33f + max / 3.0f * 2.0f);

		vec3 influence;
		influence = rgbFrom(hue, sat, bright);

*/