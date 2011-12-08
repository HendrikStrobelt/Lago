#version 330
 
in vec2 vVertex;
in vec2 vTex;

uniform vec2 move;

out vec2 fTexCoord;

void main()
{
   gl_Position = vec4(vVertex + move, 0.0, 1.0);
   fTexCoord = vec2(vTex);
}