#version 330

in vec2 vVertex;
 
void main(void) {
	gl_Position = vec4(vVertex, 0.0f, 1.0f);
}