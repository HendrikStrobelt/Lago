#version 330

uniform mat4 MVP; 
in vec2 vVertex1;
in vec2 vVertex2;
in int vDepth;
in float vWeight;

out int gDepth;
out float gWeight;
out vec4 gVertex2;

void main()
{
   gDepth = vDepth;
   gWeight = vWeight;	

   gVertex2 = MVP * vec4(vVertex2, 0, 1.0);
   gl_Position = MVP * vec4(vVertex1, 0, 1.0);
}