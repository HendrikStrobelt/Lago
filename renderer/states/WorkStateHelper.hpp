#ifndef WORK_STATE_HELPER_H
#define WORK_STATE_HELPER_H

#include "../Renderer.hpp"
#include "../../painter/GaussPainter.hpp"
#include "../../painter/DividedLinePainter.hpp"
#include "../PainterCommander.hpp"
#include "../FieldEvaluation.hpp"

enum GAUSS {VIEW, OFF};
enum COMMANDER {GAUSS_VIEW, GAUSS_OFF, DIVIDED_LINES};

class WorkStateHelper {

	public:
		WorkStateHelper(Renderer* renderer);
		~WorkStateHelper( void );

		void work( void );
		void takeOver( void );
		bool isDone( void );

		float _progress;
		GaussPainter* _gaussPainter[2];
		PainterCommander* _pc[3];
		FieldEvaluation* _fieldEvaluator[2];
		DividedLinePainter* _linePainter;

	private:
		Renderer* _r;

		void resetAll( void );

};


#endif