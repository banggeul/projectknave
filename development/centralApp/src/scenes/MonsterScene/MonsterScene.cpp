/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MonsterScene.h"


void MonsterScene::setup(){
	tempC = (int)ofRandom(0, 255);

}

void MonsterScene::update(){
	
}

void MonsterScene::draw(){
	
	
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