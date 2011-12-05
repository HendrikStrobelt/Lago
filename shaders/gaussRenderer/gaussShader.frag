#version 330

out vec4 fragColor;

flat in float fWeight;
in vec2 vTexCoord;
uniform sampler2D gaussTexture;

void main(void)
{
	vec4 col = texture(gaussTexture, vTexCoord);
	fragColor = vec4(0.0f , 0.0f, (col.b * fWeight), 1.0f);
}