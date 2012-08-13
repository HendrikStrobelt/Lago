#version 330

out vec4 fragColor;

in vec2 fTexCoord;
uniform vec3 maxValues;
uniform sampler2D dispTexture;

void main(void)
{
	vec3 normalized = texture(dispTexture, fTexCoord).rgb / maxValues;
    fragColor = vec4(normalized, 1.0f);
}