#ifndef WORK_STATE_HELPER_H
#define WORK_STATE_HELPER_H

#include "../Renderer.hpp"
#include "../../painter/GaussPainter.hpp"
#include "../PainterCommander.hpp"

enum GAUSS {VIEW};
enum COMMANDER {GAUSS_VIEW};

class WorkStateHelper {

	public:
		WorkStateHelper(Renderer* renderer);
		~WorkStateHelper( void );

		void work( void );
		void takeOver( void );

		float _progress;
		GaussPainter* _gaussPainter[1];
		PainterCommander* _pc[1];

	private:
		Renderer* _r;

};


#endif