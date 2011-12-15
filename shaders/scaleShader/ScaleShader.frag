#version 330

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D colorScheme;
uniform bool borderRun;

void main(void)
{
	if (borderRun) { 
		fragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	} else {
		fragColor = texture(colorScheme, vTexCoord);
	}
}