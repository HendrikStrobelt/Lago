#version 330
 
in vec2 vVertex;
in vec2 vTex;

out vec2 fTex;

void main()
{
   gl_Position = vec4(vVertex, 0.0, 1.0);
   fTex = vec2(vTex);
}