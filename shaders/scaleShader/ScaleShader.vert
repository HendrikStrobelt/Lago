#version 330
 
in vec2 vVertex;
in vec2 vTex;

uniform float yOffset;

out vec2 vTexCoord;

void main()
{
   gl_Position = vec4(vVertex, 0.0, 1.0);
   vTexCoord = vec2(vTex);
}