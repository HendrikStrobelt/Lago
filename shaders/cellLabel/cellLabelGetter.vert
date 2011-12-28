#version 330
 
in vec2 vVertex;
in int vLabelID;

uniform mat4 MVP;
out int gLabelID;

void main()
{
	gLabelID = vLabelID;
	gl_Position = MVP * vec4(vVertex, 0.0f, 1.0f);
}