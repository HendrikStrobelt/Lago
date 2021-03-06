#ifndef PREPARED_TEXT_H
#define PREPARED_TEXT_H

class PreparedText {

	public:
		PreparedText(int textLength, int textWidth, int textHeight, float* coords1, float* coords2, float* texCoords1, float* texCoords2, float* weight);
		~PreparedText( void );

		void setCenterPos(float centerX, float centerY);

		//pull only
		float* _coords1;
		float* _coords2;
		float* _texCoords1;
		float* _texCoords2;
		float* _weight;
		float _centerX;
		float _centerY;
		int _textLength;
		int _textPixWidth;
		int _textPixHeight;

	private:

};

#endif