#version 330

out vec4 fragColor;

in vec2 vTexCoord;
uniform float loaded;
uniform sampler2D barTexture;
uniform sampler2D barLoadedTexture;

void main(void)
{
	if (vTexCoord.x > loaded) {
		fragColor = texture(barTexture, vTexCoord);
	} else {
		fragColor = texture(barLoadedTexture, vTexCoord);		
	}
}