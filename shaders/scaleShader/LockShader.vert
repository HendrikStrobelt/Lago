#version 330
 
in vec2 vVertex;
in vec2 vTex;

uniform float xOffset;
uniform int textureSegment;

out vec2 vTexCoord;

void main()
{
   gl_Position = vec4((vVertex + vec2(xOffset, 0.0f)), 0.0, 1.0);
   vec2 texCoords = vec2(vTex.x, (vTex.y / 3.0f) + (1.0f / 3.0f) * textureSegment);

   vTexCoord = vec2(texCoords);
}