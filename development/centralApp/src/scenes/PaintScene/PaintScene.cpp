
#include "PaintScene.h"


void PaintScene::setup() {
	
	// box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
	
	// build the building for box2d
	for(int i=0; i<building.shapes.size(); i++) {
		buildingBoundsShape.push_back(ofxBox2dLine());
		buildingBoundsShape.back().clear();
		buildingBoundsShape.back().setWorld(box2d.getWorld());
		
		for(int j=0; j<building.shapes[i].pnts.size(); j++) {
			float bx = building.shapes[i].pnts[j].x;
			float by = building.shapes[i].pnts[j].y;
			ofPoint pos = getPointInPreview(bx, by);
			buildingBoundsShape.back().addPoint(pos.x, pos.y);
		}
		buildingBoundsShape.back().createShape();
	}
	
	
	
}


// ------------------------------------------------
void PaintScene::update(){
	
	
	
	// ground points
	for(int i=0; i<NUM_GROUND_PNTS; i++) {
		float n = (float)i / (float)(NUM_GROUND_PNTS-1);
		groundPnts[i].x = n * OFFSCREEN_WIDTH;
		groundPnts[i].y = OFFSCREEN_HEIGHT -  (60 + (cos((n*ofGetElapsedTimef())) * 80.0) );		
	}
	
	
}


// ------------------------------------------------
void PaintScene::draw() {
	
	// The Ground
	/*ofFill();
	ofSetColor(0, 255, 52);
	ofBeginShape();
	ofVertex(0, OFFSCREEN_HEIGHT);
	ofVertex(0, OFFSCREEN_HEIGHT-130);
	
	for(int i=0; i<NUM_GROUND_PNTS; i++) {
		ofVertex(groundPnts[i].x, groundPnts[i].y);
	}
	
	ofVertex(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT-130);
	ofVertex(OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofEndShape();
	*/
	
	
	// draw the shape of the building for box2d
	for(int i=0; i<buildingBoundsShape.size(); i++) {
		buildingBoundsShape[i].draw();
	}
	
	
	// People
	glPushMatrix();
	glTranslatef(0, 0, 0);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, i*20, 255-(i*40), 100);
		ofFill();
		ofEnableSmoothing();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++) {
			
			float x = packet.pts[i][j].x + ((OFFSCREEN_WIDTH - packet.width)/2);
			float y = (OFFSCREEN_HEIGHT-packet.height) + packet.pts[i][j].y;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
	glPopMatrix();
	
	
}

















