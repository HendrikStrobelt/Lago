#version 330

in vec2 fTexCoord;
uniform float maxValue;
uniform sampler2D gaussTex;

uniform int width;
uniform int height;
uniform bool antiAlias;

uniform bool linearMode;
uniform vec4 pointsX;
uniform vec4 pointsY;
uniform float exponent;

out vec4 fragColor;

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



vec3 getColor(vec2 texCoord) {
   
   float normalized = texture(gaussTex, texCoord).b / maxValue;
   float scaled = scale(normalized);

   vec3 fragColor_b;
   
   float l01 = 0.97;
   float l02 = 0.91;
   float l03 = 0.77;
   float l04 = 0.61;
   float l05 = 0.45;
   float l06 = 0.31;
   float l07 = 0.20;
   float l08 = 0.12;
   float l09 = 0.07;
   float l10 = 0.04;


   if (scaled < 0.0001) {
		fragColor_b = vec3(0.9, 0.9, 0.9);															//background
   } else
   if (scaled < l10) {																				//surrounding
		fragColor_b = vec3(0.7, 0.7, 0.7);
   } else
   if (scaled < l09) {
		fragColor_b = vec3((8.0 / 255.0), (48.0/255.0), (107.0/255.0));
   } else
   if (scaled < l08) {
		fragColor_b = vec3((8.0 / 255.0), (81.0/255.0), (156.0/255.0));
   } else
   if (scaled < l07) {
		fragColor_b = vec3((33.0 / 255.0), (113.0/255.0), (181.0/255.0));
   } else
   if (scaled < l06) {
		fragColor_b = vec3((66.0 / 255.0), (146.0/255.0), (198.0/255.0));
   } else
   if (scaled < l05) {
		fragColor_b = vec3((107.0 / 255.0), (174.0/255.0), (214.0/255.0));
   } else
   if (scaled < l04) {
		fragColor_b = vec3((158.0 / 255.0), (202.0/255.0), (225.0/255.0));
   } else
   if (scaled < l03) {
		fragColor_b = vec3((198.0 / 255.0), (219.0/255.0), (239.0/255.0));
   } else
   if (scaled < l02) {
		fragColor_b = vec3((222.0 / 255.0), (235.0/255.0), (247.0/255.0));
   } else
   if (scaled < l01) {
		fragColor_b = vec3((247.0 / 255.0), (251.0/255.0), (255.0/255.0));		
   } else {
		fragColor_b = vec3(1.0, 1.0, 1.0);
   }

   return fragColor_b;
}


void main(void)
{
	vec2 texCoord;

	if (antiAlias) {
		
		vec3 color;
		float stepX = 1.0f / width;
		float stepY = 1.0f / height;

		for (int run = 0; run < 4; run++) {

			switch (run) {
				case 0:
					texCoord = fTexCoord + vec2((stepX * 0.7133f), (stepY * 0.2318f));
				break;
				case 1:
					texCoord = fTexCoord + vec2((stepX * 0.2318f), (stepY * -0.7133f));				
				break;
				case 2:
					texCoord = fTexCoord + vec2((stepX * -0.7133f), (stepY * -0.2318f));				
				break;
				case 3:
					texCoord = fTexCoord + vec2((stepX * -0.2318f), (stepY * 0.7133f));				
				break;
			}
		
			color += getColor(texCoord);

		}

		fragColor = vec4((color / vec3(4,4,4)), 1);
	} else {
		texCoord = fTexCoord;
		fragColor = vec4(getColor(texCoord), 1.0f);
	}
}