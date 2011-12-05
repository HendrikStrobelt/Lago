#ifndef ISPLITABLE_PAINTER_H
#define ISPLITABLE_PAINTER_H

class ISplitablePainter {

	public:
		virtual ~ISplitablePainter( void ) {};
		
		virtual int getElementCount( void ) = 0;
		virtual void processElements(int start, int count) = 0;

	private:

};

#endif