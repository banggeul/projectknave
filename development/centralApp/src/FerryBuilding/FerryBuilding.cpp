
#include "FerryBuilding.h"



// ------------------------------------
void FerryBuilding::setupBuilding(string buildingFile) {

	building.loadImage(buildingFile);
	
}



// ------------------------------------
void FerryBuilding::draw() {

	ofSetColor(0xffffff);
	building.draw(0, 0);
	
}


		
	