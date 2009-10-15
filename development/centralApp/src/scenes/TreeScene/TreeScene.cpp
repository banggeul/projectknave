
#include "TreeScene.h"

// ------------------------------------------
void TreeScene::setup() {
	curStem = 0;
}

// ------------------------------------------

void TreeScene::mousePressed(int x, int y, int button) {
	stems[curStem].initStem(x, y);
	curStem ++;
	curStem %= MAX_STEMS;
}

// ------------------------------------------
void TreeScene::update() {

	for(int i=0; i<MAX_STEMS; i++) {

		if(stems[i].bALive) {

			for(int j=0; j<building.shapes.size(); j++) {
				for(int k=0; k<building.shapes[j].pnts.size(); k++) {

					ofxVec2f cntPnt(building.shapes[j].pnts[k].x, building.shapes[j].pnts[k].y);

					//void particle::addAttractionForce(float x, float y, float radius, float scale){

					stems[i].head.addRepulsionForce(cntPnt.x,
													cntPnt.y,
													30.0,
													30.0);
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

	for(int i=0; i<MAX_STEMS; i++) {
		stems[i].draw();
	}

	for(int j=0; j<building.shapes.size(); j++) {
		for(int k=0; k<building.shapes[j].pnts.size(); k++) {

		}
	}

	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	for (int i = 0; i < packet.nBlobs; i++){
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofEnableSmoothing();
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++){
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();
	}


}
