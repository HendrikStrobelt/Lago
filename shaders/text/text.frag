#version 330

in vec2 fTexCoord;
in vec3 fColor;
out vec4 fragColor;

uniform sampler2D tex;

void main(void) {
  fragColor = vec4(1, 1, 1, texture2D(tex, fTexCoord).r) * vec4(fColor, 1.0f);
}