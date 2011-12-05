#version 330

out vec4 fragColor;

in vec2 fTexCoord;
uniform vec3 maxValues;
uniform sampler2D texture;

void main(void)
{
	vec3 normalized = texture(texture, fTexCoord).rgb / maxValues;
    fragColor = vec4(normalized, 1.0f);
}