#include "../Cinema.hpp"

namespace testScript3 {

	void setScript(Cinema* cinema) {
		cinema->addPanning(0,79.787,17.4217);
		cinema->addZoomChange(0.5,6);
		cinema->addSideChange(3.5,12);
		cinema->addSideChange(6.5,16);

		cinema->addStopEvent(8,40);

		cinema->addRightClickM(10.0, 384,457);
		cinema->addLabelClick(11.5, true, 1);
		cinema->addRightClickM(13, 316,373);
		cinema->addLabelClick(14.5, true, 5);
		cinema->addRightClickM(16, 869,281);
		cinema->addEmptyClick(17.5);
		cinema->addRightClickM(19, 928,446);
		cinema->addLabelClick(20.5, true, 53);
		cinema->addEmptyClick(20.5);

		cinema->addEmptyEvent(24.0);

	}

};