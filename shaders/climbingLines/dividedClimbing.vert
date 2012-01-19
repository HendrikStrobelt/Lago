#version 330

in vec2 vVertex1;
in vec2 vVertex2;
in int vAlpha;
in float vWeight;

out float gWeight;
out int gAlpha;
out vec4 gVertex2;

void main()
{
   gAlpha = vAlpha;
   gWeight = vWeight;	
   gVertex2 = vec4(vVertex2, 0, 1.0);
   gl_Position = vec4(vVertex1, 0, 1.0);
}