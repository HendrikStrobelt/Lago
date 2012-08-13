#version 330

in vec2 vVertex;
in int vDepth;
in float vWeight;

out int gDepth;
out float gWeight;

void main() 
{
   gDepth = vDepth;
   gWeight = vWeight;
   gl_Position = vec4(vVertex, 0, 1.0);
}