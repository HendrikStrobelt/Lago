#version 330

out vec4 fragColor;

in vec2 vTexCoord;

uniform sampler2D tex;


void main(void)
{
	fragColor = texture(tex, vTexCoord);		
}