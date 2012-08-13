#version 330

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D lockTexture;

void main(void)
{
	fragColor = texture(lockTexture, vTexCoord);
}