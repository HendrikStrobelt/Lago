#include "../Cinema.hpp"

namespace AirTrafficScript {

	void setScript(Cinema* cinema) {

		double time = 0;
		time += 1.0;

		cinema->addPressedMovement(time, time+3, 106.128, 17.3519);
		time += 3;

		cinema->addPanning(time, 106.128, 17.3519);
		time += 1;
		
		cinema->addZoomChange(time, 8);
		time += 4;
		
		cinema->addZoomChange(time, 10);
		time += 4;

		cinema->addPressedMovement(time, time+0.5, 106.94,16.9109);
		time += 0.5;

		cinema->addPanning(time, 106.94,16.9109);
		time += 1;
		
		cinema->addZoomChange(time, 14);
		time += 4;

		cinema->addZoomChange(time, 18);
		time += 4;


	}

};