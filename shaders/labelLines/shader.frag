#version 330

uniform sampler2D colorScheme;

out vec4 fragColor;

void main(void) {
	fragColor = vec4(texture(colorScheme, vec2(0.5f, 0.5f)).rgb, 0.5f);
}