#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {
	curStem = 0;

	building.setupBuilding("buildingRefrences/buidlingFiles/treeFerryContour.xml");

}

// ------------------------------------------

void TreeScene::mousePressed(int x, int y, int button) {
	stems[curStem].initStem(x, y);
	curStem ++;
	curStem %= MAX_STEMS;

	// events for the ferry building
	building.mousePressed(x, y, button);
}


//--------------------------------------------------------------
void TreeScene::keyPressed(int key) {

	// keys for the ferry
	building.keyPressed(key);

}

// ------------------------------------------
void TreeScene::update() {
	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

	for (int i = 0; i < MAX_STEMS; i++) {

		if (stems[i].bALive) {


			for (int j = 0; j < building.shapes.size(); j++) {
				FerryShape& cur = building.shapes[j];
				for (int k = 0; k < cur.pnts.size() - 1; k++) {
					ofxPoint2f start = cur.pnts[k];
					ofxPoint2f stop = cur.pnts[k + 1];
					for(float q = 0; q < 1; q += .1) {
						ofxPoint2f interp = start.interpolate(stop, q);
						stems[i].head.addRepulsionForce(interp.x, interp.y, 30.0, 30.0);
					}
				}
			}

			for(int j = 0; j < packet.nBlobs; j++) {
				for(int k = 0; k < packet.nPts[j]; k++) {
					ofPoint& cur = packet.pts[j][k];
					stems[i].head.addAttractionForce(cur.x * scalex, cur.y * scaley, 200, 1);
				}
			}

			stems[i].update();
		}
	}
}

// ------------------------------------------
void TreeScene::draw() {


	// draw the building
	building.drawContour();

	for (int i = 0; i < MAX_STEMS; i++) {
		stems[i].draw();
	}

	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	for (int i = 0; i < packet.nBlobs; i++) {
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofEnableSmoothing();
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++) {
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();
	}
}
