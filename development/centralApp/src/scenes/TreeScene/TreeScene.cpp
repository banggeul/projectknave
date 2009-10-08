
#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {

}

// ------------------------------------------

void TreeScene::mousePressed(int x, int y, int button) {
}

// ------------------------------------------
void TreeScene::update() {
	
}

// ------------------------------------------
void TreeScene::draw() {
	
	
	
	
	for (int i = 0; i < packet.nBlobs; i++){
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			ofVertex(packet.pts[i][j].x, packet.pts[i][j].y);
		}
		ofEndShape(true);
	}
}