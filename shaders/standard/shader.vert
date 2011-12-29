#version 330

in vec2 vVertex;
uniform mat4 MVP;
 
void main(void) {
	gl_Position = MVP * vec4(vVertex, 0.0f, 1.0f);
}