#version 330
 
in vec2 vVertex;
in vec2 vTex;

out vec2 fTexCoord;

in mat4 MVP;
in vec4 vOldBox;
uniform bool blend;
out vec2 fOldTexCoords;



vec2 getOldTexCoords(vec2 pos) {
	vec2 leftBot = (MVP * vec4(vOldBox.xy, 0.0f, 1.0f)).xy;
	vec2 rightUp = (MVP * vec4(vOldBox.zw, 0.0f, 1.0f)).xy;

	float x = (pos.x - leftBot.x) / (rightUp.x - leftBot.x);
	float y = (pos.y - leftBot.y) / (rightUp.y - leftBot.y);
	
	return vec2(x,y);
}

void main()
{
   gl_Position = vec4(vVertex, 0.0, 1.0);
   fTexCoord = vec2(vTex);

   if (blend) {
		fOldTexCoords = getOldTexCoords(vVertex);
   } else {
		fOldTexCoords = vec2(-1.0f,-1.0f);
   }
}