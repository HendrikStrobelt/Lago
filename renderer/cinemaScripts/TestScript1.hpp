#include "../Cinema.hpp"

namespace testScript1 {

	void setScript(Cinema* cinema) {
		cinema->addPressedMovementM(1,3,-510,372);
		cinema->addPanning(3, -771.12,-562.464);
		cinema->addZoomChange(4, 3);
		cinema->addSideChange(7,10);
		cinema->addPressedMovementM(10,12,179,-301);
		cinema->addPanning(12,-647.93, -355.312);
		cinema->addZoomChange(13,2);
		cinema->addEmptyEvent(18);
	}

};