/*
 *  sceneManager.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "sceneManager.h"


void sceneManager::setup(cvManager& CVM){
	this->CVM = &CVM;

	currentScene	= 0;
	numScenes		= 6;

	scenes			= new baseScene*[numScenes];

	scenes[0]		= new HandScene();				// OOP love it.
	scenes[1]		= new TreeScene();
	scenes[2]		= new PaintScene();
	scenes[3]		= new MonsterScene();
	scenes[4]		= new StarScene();
	scenes[5]   = new WarpScene();


	// set the blob listener
	tracker.setListener(this);

	for (int i = 0; i < numScenes; i++){
		scenes[i]->setup();
		scenes[i]->tracker = &tracker;		// <--- ref to the tracker - werd
	}
}

void sceneManager::nextScene(){
	gotoScene(currentScene + 1);
}


void sceneManager::prevScene(){
	gotoScene(currentScene - 1);
}

void sceneManager::passInPacket(computerVisionPacket * packet){

	memcpy(&(scenes[currentScene]->packet), packet, sizeof(computerVisionPacket));


	// this is where we update the blob tracker
	networkBlobs.clear();
	networkBlobs.assign(packet->nBlobs, ofxCvBlob());

	for(int i=0; i<packet->nBlobs; i++) {

		networkBlobs[i].hole		 = packet->bAmInner[i];
		networkBlobs[i].boundingRect = packet->rect[i];
		networkBlobs[i].centroid = packet->centroid[i];
		networkBlobs[i].nPts = packet->nPts[i];
		networkBlobs[i].pts.assign(packet->nPts[i], ofPoint());

		for (int j = 0; j < packet->nPts[i]; j++) {

			float x = packet->pts[i][j].x;
			float y = packet->pts[i][j].y;
			networkBlobs[i].pts[j].set(x, y);

		}
	}

	// go and track them
	tracker.trackBlobs(networkBlobs);


}


// ---------------------------------------------------
void sceneManager::update(){
	scenes[currentScene]->update();
}

void sceneManager::draw(){
	ofPushStyle();
	scenes[currentScene]->draw();
	ofPopStyle();
}

void sceneManager::drawTop(){
	scenes[currentScene]->drawTop();
}

void sceneManager::gotoScene(int theScene) {
	if(theScene < 0) theScene = 0;
	if(theScene >= numScenes) theScene = numScenes-1;
	currentScene = theScene;

	// clean up the scenes
	scenes[currentScene]->cleanUpScene();
	tracker.blobs.clear();

	ofxDaito::scene(currentScene);

	switch(currentScene) {
		case HAND_SCENE: ofSetFrameRate(20); break;
		case PAINT_SCENE: ofSetFrameRate(30); break;
		case STAR_SCENE: ofSetFrameRate(45); break;
		case MONSTER_SCENE: ofSetFrameRate(30); break;
		case WARP_SCENE: ofSetFrameRate(30); break;
		default: ofSetFrameRate(60);
	}

	CVM->setScene(currentScene);
}


// --------------------------------------------------- keyboard and mouse
void sceneManager::keyPressed(int key) {
	scenes[currentScene]->keyPressed(key);
}
void sceneManager::keyReleased(int key) {
	scenes[currentScene]->keyReleased(key);
}
void sceneManager::mouseMoved(int wx, int wy, int x, int y) {
	scenes[currentScene]->mouseMoved(wx, wy, x, y);
	scenes[currentScene]->mouseX = x;
	scenes[currentScene]->mouseY = y;
}
void sceneManager::mouseDragged(int wx, int wy, int x, int y, int button) {
	scenes[currentScene]->mouseDragged(wx, wy, x, y, button);
}
void sceneManager::mousePressed(int wx, int wy, int x, int y, int button) {
	scenes[currentScene]->mousePressed(wx, wy, x, y, button);
}
void sceneManager::mouseReleased(int wx, int wy, int x, int y, int button) {
	scenes[currentScene]->mouseReleased(wx, wy, x, y, button);
}


// --------------------------------------------------- tracker events
void sceneManager::blobOn( int x, int y, int bid, int order ) {
	scenes[currentScene]->blobOn(x, y, bid, order);
}
void sceneManager::blobMoved( int x, int y, int bid, int order ) {
	scenes[currentScene]->blobMoved(x, y, bid, order);
}
void sceneManager::blobOff( int x, int y, int bid, int order ) {
	scenes[currentScene]->blobOff(x, y, bid, order);
}











