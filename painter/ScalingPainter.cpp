#include "ScalingPainter.hpp"		
#include "../context/Context.hpp"
#include "../text/TextRenderer.hpp"
#include <sstream>
#include <iomanip>

GLSLShader* ScalingPainter::_shader_ptr = NULL;

ScalingPainter::ScalingPainter( void ) {
	test = new TextRenderer("C://Windows//Fonts//verdana.ttf", 9);

	_maxN = -1;
	_maxE = -1;
	_e05 = -1;
	_n05 = -1;

	context::addResizeListener(this);
	createShader();
	initVao();
}


ScalingPainter::~ScalingPainter( void) {
	glDeleteBuffers(2, &_vbo[0]);
	glDeleteVertexArrays(1, &_vao);
	delete test;
}

//static clean up
void ScalingPainter::cleanUp( void ) {
	delete _shader_ptr;
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

		test->clearTextStorage();
		test->addText(getFloatString(0), _textAnchors[0][0], _textAnchors[0][1]);//right low
		if (n05 < 0.0f) { //error code no middle value
			test->addText("", _textAnchors[1][0], _textAnchors[1][1]);
		} else {
			test->addText(getFloatString(n05), _textAnchors[1][0], _textAnchors[1][1]);
		}
		test->addText(getFloatString(_maxN), _textAnchors[2][0], _textAnchors[2][1]);
		
		if (hasEdges) {
			test->addText(getFloatString(0), _textAnchors[3][0], _textAnchors[3][1]);//left low
			if (e05 < 0.0f) { //error code no middle value
				test->addText("", _textAnchors[4][0], _textAnchors[4][1]);
			} else {
				test->addText(getFloatString(e05), _textAnchors[4][0], _textAnchors[4][1]);
			}
			test->addText(getFloatString(_maxE), _textAnchors[5][0], _textAnchors[5][1]);
		}
	}


	test->renderText(Color(0.0f, 0.0f, 0.0f, 1.0f));

	glBlendFunc(GL_ONE, GL_ZERO);

	glBindVertexArray(_vao);
		_shader_ptr->use();
					glUniform1i(_shader_ptr->getUniformLocation("colorScheme"), 0);
					glUniform1i(_shader_ptr->getUniformLocation("adaptiveScaleBars"), context::_options._adaptiveScaleBars);

					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), true);
					glDrawArrays(GL_TRIANGLE_STRIP,  8, 4); 
					if (hasEdges) {
						glDrawArrays(GL_TRIANGLE_STRIP, 12, 4); 
					}
			glBindTexture(GL_TEXTURE_2D, context::_options._nodeScheme);
					int scaleMode = 0;
					float* cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);

					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
	if (hasEdges) {
			glBindTexture(GL_TEXTURE_2D, context::_options._edgeScheme);		
					scaleMode = 1;
					cp = &(context::_scaleOptions[scaleMode]._controlPoints[0][0]);
					glUniform1i(_shader_ptr->getUniformLocation("linearMode"), context::_scaleOptions[scaleMode]._linearMode);
					glUniform4f(_shader_ptr->getUniformLocation("pointsX"), cp[0], cp[2], cp[4], cp[6]);
					glUniform4f(_shader_ptr->getUniformLocation("pointsY"), cp[1], cp[3], cp[5], cp[7]);
					glUniform1f(_shader_ptr->getUniformLocation("exponent"), context::_scaleOptions[scaleMode]._exponent);

					glUniform1i(_shader_ptr->getUniformLocation("borderRun"), false);
					glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); 
	}
			glBindTexture(GL_TEXTURE_2D,  0);
		_shader_ptr->unUse();
	glBindVertexArray(0);
	


}

void ScalingPainter::resizeEvent(int width, int height) {
	updateBarPositions(width, height);
}

//private stuff


string ScalingPainter::getFloatString(float value) {
	stringstream ss;
	ss << right << setw(9) << setprecision(3) << value;
	string a = ss.str();
	return ss.str();
}

void ScalingPainter::updateBarPositions(int w, int h) {
	float stepX = 2.0f / (float)w;
	float stepY = 2.0f / (float)h;

	#define right 70.0f
	#define lower 15.0f
	#define width 10.0f
	#define height 260.0f
	#define	shift 18.0f

	float textureQuad[32] =   { (1.0f - (right)         * stepX), (-1.0f + (lower + height) * stepY),
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

	glBindBuffer (GL_ARRAY_BUFFER, _vbo[VERTEX]);
		glBufferData (GL_ARRAY_BUFFER, 32 * sizeof(float), &textureQuad[0], GL_STATIC_DRAW);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
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
	if (_shader_ptr == NULL) {
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

		_shader_ptr = new GLSLShader(attribs, unis, "shaders/scaleShader/ScaleShader.vert", "shaders/ScaleShader/ScaleShader.frag");
	}
}


void ScalingPainter::initVao( void ) {

	float texture[32] = { 0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f, //fill parts
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f,
						  0.0f, 1.0f,     0.0f,  0.0f,      1.0f, 1.0f,     1.0f,  0.0f};
	glGenVertexArrays(1, &_vao);
	glGenBuffers (2, &_vbo[0]);


	glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
		glBufferData (GL_ARRAY_BUFFER, 32 * sizeof(float), &texture[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(_vao);	 
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTEX]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vVertex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vVertex"), 2, GL_FLOAT, GL_FALSE, 0, 0);	
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEX_COORDS]);
			glEnableVertexAttribArray(_shader_ptr->getAttributeLocation("vTex"));
			glVertexAttribPointer (_shader_ptr->getAttributeLocation("vTex"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int w,h;
	context::getWindowSize(&w, &h);
	updateBarPositions(w,h);
}
