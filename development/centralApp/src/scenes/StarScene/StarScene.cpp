/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "StarScene.h"


void StarScene::setup(){
	tempC = (int)ofRandom(0, 255);
	PM.setupWorld();


	temp.loadImage("images/logoHalf.png");
	temp.setImageType(OF_IMAGE_GRAYSCALE);
	PM.findLockTarget(temp.getPixels(), temp.width, temp.height);
}

void StarScene::update(){



	// always move 200 particles OFFSCREEN
	for (int i = 0; i < 200; i++){
		PM.particles[i].pos.set(OFFSCREEN_WIDTH*2 + i * 10, 500,0);
	}

	PM.update();


	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;

	for (int i = 0; i < packet.nCentroids; i++){
		PM.particles[i].pos.set(packet.allCentroid[i].x * scalex, packet.allCentroid[i].y * scaley, 0);
		PM.particles[i].vel.set(0,0,0);
		PM.particles[i].energy = 1;						// BIG
	}






}

void StarScene::announceChange(int change){
	ofxOscMessage msg;
	msg.setAddress("/bang");								//	bang
	msg.addStringArg("internalSceneChange");					//	handTrigger
	msg.addIntArg(4);									//	SCENE 4 (?)
	msg.addIntArg(change);									//	SCENE 4 (?)
	ofxDaito::sendCustom(msg);
}




void StarScene::keyPressed(int key){

	switch (key){
		case '0': announceChange(0); PM.mode = 0; break;
		case '1': announceChange(1); PM.mode = 1; break;
		case '2': announceChange(2); PM.mode = 2; break;
		case '3': announceChange(3); PM.mode = 3; break;
		case '4': announceChange(4); PM.mode = 4; break;
		case '5': announceChange(5); PM.mode = 5; break;
	}





}

void StarScene::draw(){


	PM.draw();
}
