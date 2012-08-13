#ifndef LABEL_ORDER_COM_H
#define LABEL_ORDER_COM_H

using namespace std;

#include <vector>

namespace labelOrderCom {

	struct MovedBox{
		int id;
		int x;
		int y;
	};

	void prepare( void );
	void add(int centerX, int centerY, int width, int height);
	
	vector<MovedBox>* transmit( void );

};


#endif