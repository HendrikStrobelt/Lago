#include "../Cinema.hpp"

namespace testScript2 {

	void setScript(Cinema* cinema) {
		cinema->addPressedMovement(0.5,1.0,-2.92389,-1.92491);
		cinema->addPanning(1.0,-2.92389,-1.92491);
		cinema->addZoomChange(2.0,2);
		cinema->addZoomChange(3.0,4);
		cinema->addZoomChange(4.0,6);
		cinema->addZoomChange(5.0,8);
		cinema->addZoomChange(6.0,10);

		cinema->addZoomChange(9.0,18);
		cinema->addZoomChange(12.0,30);
		cinema->addPressedMovement(15.0,15.5,-2.92389,-1.92491);
		cinema->addPanning(15.5,-2.92389,-1.92491);
		cinema->addSideChange(16.0,1);

		cinema->addEmptyEvent(20);
	}

};