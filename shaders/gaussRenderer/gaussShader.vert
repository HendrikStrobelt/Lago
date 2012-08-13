#version 330

in vec2 vVertex;


void main() 
{
   gl_Position = vec4(vVertex, 0, 1.0);
}