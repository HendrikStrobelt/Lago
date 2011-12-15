#version 330

in vec4 vCoord1;
in vec4 vTexCoord1;
in vec4 vCoord2;
in vec4 vTexCoord2;
in vec3 vColor;

out vec4 gCoord1;
out vec4 gCoord2;
out vec4 gTexCoord1;
out vec4 gTexCoord2;
out vec3 gColor;
 
void main(void) {
  gColor = vColor;
  gCoord1 = vCoord1;
  gCoord2 = vCoord2;
  gTexCoord1 = vTexCoord1;
  gTexCoord2 = vTexCoord2;
}