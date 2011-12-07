#ifndef WORK_STATE_HELPER_H
#define WORK_STATE_HELPER_H

#include "../Renderer.hpp"
#include "../../painter/GaussPainter.hpp"
#include "../PainterCommander.hpp"
#include "../FieldEvaluation.hpp"

enum GAUSS {VIEW};
enum COMMANDER {GAUSS_VIEW};

class WorkStateHelper {

	public:
		WorkStateHelper(Renderer* renderer);
		~WorkStateHelper( void );

		void work( void );
		void takeOver( void );
		bool isDone( void );

		float _progress;
		GaussPainter* _gaussPainter[1];
		PainterCommander* _pc[1];
		FieldEvaluation* _fieldEvaluator;

	private:
		Renderer* _r;

};


#endif