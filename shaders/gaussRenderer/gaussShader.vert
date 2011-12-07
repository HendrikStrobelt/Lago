#version 330

uniform mat4 MVP; 
in vec2 vVertex;

in int vDepth;
in float vWeight;
out int gDepth;
out float gWeight;

void main() 
{
   gDepth = vDepth;
   gWeight = vWeight;
   gl_Position = MVP * vec4(vVertex, 0, 1.0);
}