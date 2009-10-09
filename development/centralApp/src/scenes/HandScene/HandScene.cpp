/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "HandScene.h"


void HandScene::setup(){
	tempC = (int)ofRandom(0, 255);
}

void HandScene::update(){
	
}

void HandScene::draw(){
	
	
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
	
	ofSetColor(255, 0, 0);
	ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);

}