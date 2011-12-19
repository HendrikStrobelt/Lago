#version 330

in vec2 fTexCoord;
in float fWeight;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D colorScheme;
uniform vec4 color;


void main(void) {
	if (color.r == -1.0f) {
		//no color set use colorScheme
		fragColor = vec4(1, 1, 1, texture2D(tex, fTexCoord).r) * texture2D(colorScheme, vec2(fWeight, 0.5f));
	} else {
		fragColor = vec4(1, 1, 1, texture2D(tex, fTexCoord).r) * color;
	}
  
}