#include "ScalingPainter.hpp"		
#include "../context/Context.hpp"
#include "../text/TextRenderer.hpp"
#include "../helper/EnvironmentHelper.hpp"
#include "../helper/MouseHandler.hpp"
#include <sstream>
#include <iomanip>

GLSLShader* ScalingPainter::_b_shader_ptr = NULL;
GLSLShader* ScalingPainter::_l_shader_ptr = NULL;
GLuint ScalingPainter::_lockTexture = -1;

ScalingPainter::ScalingPainter( void ) {
	text = new TextRenderer("C://Windows//Fonts//verdana.ttf", 9);

	_maxN = -1;
	_maxE = -1;
	_e05 = -1;
	_n05 = -1;

	context::addResizeListener(this);
	createShader();
	initVao();
}


ScalingPainter::~ScalingPainter( void) {
	glDeleteBuffers(4, &_vbo[0]);
	glDeleteVertexArrays(2, &_vao[0]);
	glDeleteTextures(1, &_lockTexture);
	delete text;
}

//static clean up
void ScalingPainter::cleanUp( void ) {
	delete _b_shader_ptr;
	delete _l_shader_ptr;
}

void ScalingPainter::renderScaleBars(float maxVals[], bool hasEdges) {

	float maxN = maxVals[0];
	float maxE = 0.0f;
	if (hasEdges) {
		maxE = maxVals[1];
	}

	float n05, e05;

	if (context::_options._adaptiveScaleBars) {
		n05 = 0.5f * _maxN;
		e05 = 0.5f * _maxE;
	} else {
		ScaleOptions* s = &context::_scaleOptions[0];
		float x[] = {s->_controlPoints[0][0], s->_controlPoints[1][0], s->_controlPoints[2][0], s->_controlPoints[3][0]};
		float y[] = {s->_controlPoints[0][1], s->_controlPoints[1][1], s->_controlPoints[2][1], s->_controlPoints[3][1]};
		n05 = reverseScale(0.5f, s->_linearMode, s->_exponent, x, y) * _maxN;

		s = &context::_scaleOptions[1];
		float x2[] = {s->_controlPoints[0][0], s->_controlPoints[1][0], s->_controlPoints[2][0], s->_controlPoints[3][0]};
		float y2[] = {s->_controlPoints[0][1], s->_controlPoints[1][1], s->_controlPoints[2][1], s->_controlPoints[3][1]};
		e05 = reverseScale(0.5f, s->_linearMode, s->_exponent, x2, y2) * _maxE;
	}


	if (maxN != _maxN || maxE != _maxE || n05 != _n05 || e05 != _e05) {
		_maxN = maxN;
		_maxE = maxE;
		_n05 = n05;
		_e05 = e05;

		text->clearTextStorage();
		text->addText(getFloatString(0), _textAnchors[0][0], _textAnchors[0][1]);//right low
		if (n05 < 0.0f) { //error code no middle value
			text->addText("", _textAnchors[1][0], _textAnchors[1][1]);
		} else {
			text->addText(getFloatString(n05), _textAnchors[1][0], _textAnchors[1][1]);
		}
		text->addText(getFloatString(_maxN), _textAnchors[2][0], _textAnchors[2][1]);
		
		if (hasEdges) {
			text->addText(getFloatString(0), _textAnchors[3][0], _textAnchors[3][1]);//left low
			if (e05 < 0.0f) { //error code no middle value
				text->addText("", _textAnchors[4][0], _textAnchors[4][1]);
			} else {
				text->addText(getFloatString(e05), _textAnchors[4][0], _textAnchors[4][1]);
			}
			text->addText(getFloatString(_maxE), _textAnchors[5][0], _textAnchors[5][1]);
		}
	}


	text->renderText(Color(0.0f, 0.0f, 0.0f, 1.0f));

	glBlendFunc(GL_ONE, GL_ZERO);

	glBindVertexArray(_vao[BAR]);
		_b_shader_ptr->use();
					glUniform1i(_b_shader_ptr->getUniformLocation("colorScheme"), 0);
					glUniform1i(_b_shader_ptr->getUniformLocation("adaptiveScaleBars"), context::_options._adaptiveScaleBars);

					glUniform1i(_b_shader_ptr->getUniformLocation("borderRun"), true);
					glDrawArrays(GL_TRIANGLE_STRIP,  8, 4); 
					if (hasEdges) {
						glDrawArrays(GL_TRIANGLE_STRIP, 12, 4); 
					}
			glBindTexture(GL_TEXTURE_2D, context::_options._nodeScheme);
					int scaleMode = 0;
					float* cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_b_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_b_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_b_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_b_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);

					glUniform1i(_b_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	if (hasEdges) {
			glBindTexture(GL_TEXTURE_2D, context::_options._edgeScheme);		
					scaleMode = 1;
					cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_b_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_b_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_b_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_b_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);

					glUniform1i(_b_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); 
	}
			glBindTexture(GL_TEXTURE_2D,  0);
		_b_shader_ptr->unUse();
	glBindVertexArray(0);
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//render lock
	glBindVertexArray(_vao[LOCK]);
		_l_shader_ptr->use();
			glBindTexture(GL_TEXTURE_2D,  _lockTexture);
				if (hasEdges) {
					glUniform1f(_l_shader_ptr->getUniformLocation("xOffset"), -_lockXShift);
				} else {
					glUniform1f(_l_shader_ptr->getUniformLocation("xOffset"), 0.0f);
				}
				if (context::_options._lock) { 
					if (context::_options._overLock) {
						glUniform1i(_l_shader_ptr->getUniformLocation("textureSegment"), 0);
					} else {
						glUniform1i(_l_shader_ptr->getUniformLocation("textureSegment"), 1);
					}
				} else {
					glUniform1i(_l_shader_ptr->getUniformLocation("textureSegment"), 2);
				}
				glUniform1i(_l_shader_ptr->getUniformLocation("lockTexture"), 0);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindTexture(GL_TEXTURE_2D,  0);
		_l_shader_ptr->unUse();
	glBindVertexArray(0);
}

void ScalingPainter::resizeEvent(int width, int height) {
	text->resize(width, height);
	updatePositions(width, height);
}

//private stuff


string ScalingPainter::getFloatString(float value) {
	stringstream ss;
	ss << right << setw(9) << setprecision(3) << value;
	string a = ss.str();
	return ss.str();
}

void ScalingPainter::updatePositions(int w, int h) {
	float stepX = 2.0f / (float)w;
	float stepY = 2.0f / (float)h;

	#define right 70.0f
	#define lower 15.0f
	#define width 10.0f
	#define height 260.0f
	#define	shift 18.0f

	#define lockWidth 29.0f
	#define lockHeight 28.0f
	#define h_lockWidth 14.5f
	#define h_lockHeight 14.0f

	float barQuad[32] =       { (1.0f - (right)         * stepX), (-1.0f + (lower + height) * stepY),
								(1.0f - (right + width) * stepX), (-1.0f + (lower + height) * stepY),
								(1.0f - (right)         * stepX), (-1.0f + (lower)          * stepY),
								(1.0f - (right + width) * stepX), (-1.0f + (lower)          * stepY),
								(1.0f - (right + shift)         * stepX), (-1.0f + (lower + height) * stepY),
								(1.0f - (right + width + shift) * stepX), (-1.0f + (lower + height) * stepY),
								(1.0f - (right + shift)         * stepX), (-1.0f + (lower)          * stepY),
								(1.0f - (right + width + shift) * stepX), (-1.0f + (lower)          * stepY),
								
	/* border stuff */			(1.0f - (right-2)         * stepX), (-1.0f + (lower+2 + height) * stepY),
								(1.0f - (right+2 + width) * stepX), (-1.0f + (lower+2 + height) * stepY),
								(1.0f - (right-2)         * stepX), (-1.0f + (lower-2)          * stepY),
								(1.0f - (right+2 + width) * stepX), (-1.0f + (lower-2)          * stepY),
								(1.0f - (right-2 + shift)         * stepX), (-1.0f + (lower+2 + height) * stepY),
								(1.0f - (right+2 + width + shift) * stepX), (-1.0f + (lower+2 + height) * stepY),
								(1.0f - (right-2 + shift)         * stepX), (-1.0f + (lower-2)          * stepY),
								(1.0f - (right+2 + width + shift) * stepX), (-1.0f + (lower-2)          * stepY)};

	float lockQuad[8] = { 1.0f - (right + width / 2.0f + h_lockWidth) * stepX, (-1.0f + (lower + height + h_lockHeight + lockHeight) * stepY),
						  1.0f - (right + width / 2.0f + h_lockWidth) * stepX, (-1.0f + (lower + height + h_lockHeight             ) * stepY),
						  1.0f - (right + width / 2.0f - h_lockWidth) * stepX, (-1.0f + (lower + height + h_lockHeight + lockHeight) * stepY),
						  1.0f - (right + width / 2.0f - h_lockWidth) * stepX, (-1.0f + (lower + height + h_lockHeight             ) * stepY)};
	
	_lockXShift = (shift / 2.0f) * stepX;


	//calculate the text anchors
	_textAnchors[0][0] = (1.0 - (right/2.0f) * stepX);       _textAnchors[0][1] = (-1.0 + (lower) * stepY);
	_textAnchors[1][0] = (1.0 - (right/2.0f) * stepX);       _textAnchors[1][1] = (-1.0 + (lower + (height / 2.0f)) * stepY);
	_textAnchors[2][0] = (1.0 - (right/2.0f) * stepX);       _textAnchors[2][1] = (-1.0 + (lower + height) * stepY);
	_textAnchors[3][0] = (1.0 - (right + shift + width + (right/2.0f)) * stepX);       _textAnchors[3][1] = (-1.0 + (lower) * stepY);
	_textAnchors[4][0] = (1.0 - (right + shift + width + (right/2.0f)) * stepX);       _textAnchors[4][1] = (-1.0 + (lower + (height / 2.0f)) * stepY);
	_textAnchors[5][0] = (1.0 - (right + shift + width + (right/2.0f)) * stepX);       _textAnchors[5][1] = (-1.0 + (lower + height) * stepY);


	for (int i = 0; i < 6; i++) {
		_textAnchors[i][0] = (_textAnchors[i][0] + 1.0f) / 2.0f;
		_textAnchors[i][1] = (_textAnchors[i][1] + 1.0f) / 2.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[B_VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), &barQuad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[L_VERTEX]);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &lockQuad[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mouseHandler::setLock((lockQuad[3] + 1.0f) / 2.0f, (lockQuad[2] + 1.0f) / 2.0f, (lockQuad[5] + 1.0f) / 2.0f, (lockQuad[4] + 1.0f) / 2.0f, _lockXShift / 2.0f);
}

float ScalingPainter::reverseScale(float normedVal, bool linearMode, float exponent, float pointsX[], float pointsY[]) {
	if (linearMode) {
		//linear mode
		
		//not supported only few combinded linear functions can be inverted
		//and its in general to much effort
		return -1.0f;

	} else {
		//exponential mode
		return pow(normedVal, 1.0f / exponent); 	
	}
}

void ScalingPainter::createShader( void ) {
	if (_b_shader_ptr == NULL) {
		vector<string> unis;
		vector<string> attribs;

		attribs.push_back("vTex");
		attribs.push_back("vVertex");
		unis.push_back("colorScheme");
		unis.push_back("borderRun");
		unis.push_back("adaptiveScaleBars");

		unis.push_back("linearMode");
		unis.push_back("pointsX");
		unis.push_back("pointsY");
		unis.push_back("exponent");

		_b_shader_ptr = new GLSLShader(attribs, unis, "shaders/scaleShader/ScaleShader.vert", "shaders/ScaleShader/ScaleShader.frag");

		unis.clear();
		unis.push_back("lockTexture");
		unis.push_back("xOffset");
		unis.push_back("textureSegment");

		_l_shader_ptr = new GLSLShader(attribs, unis, "shaders/scaleShader/LockShader.vert", "shaders/ScaleShader/LockShader.frag");

		_lockTexture = envHelper::loadRGBTexture("_Tex/locks.tga");
	}
}


void ScalingPainter::initVao( void ) {

	float texture[32] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f, //fill parts
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};
	glGenVertexArrays(2, &_vao[0]);
	glGenBuffers (4, &_vbo[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[B_TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 32 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[L_TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 8 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(_vao[BAR]);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[B_VERTEX]);
			glEnableVertexAttribArray(_b_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_b_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[B_TEX_COORDS]);
			glEnableVertexAttribArray(_b_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_b_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(_vao[LOCK]);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[L_VERTEX]);
			glEnableVertexAttribArray(_l_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_l_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[L_TEX_COORDS]);
			glEnableVertexAttribArray(_l_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_l_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int w,h;
	context::getWindowSize(&w, &h);
	updatePositions(w,h);
}
