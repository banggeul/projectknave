/*
 *  PaintScene.cpp
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 08/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "PaintScene.h"

void PaintScene::setup(){
	tempC = (int)ofRandom(0, 255);
	
}

void PaintScene::update(){
	
}

void PaintScene::draw(){
	
	
	ofFill();
	ofSetColor(tempC, tempC, 255);
	ofRect(0,0,320,240);
	
	
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