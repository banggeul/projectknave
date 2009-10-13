#include "testApp.h"

void testApp::setup(){
	fern.addLeaf("Fern_Leaf_001.svg");
	fern.addLeaf("Fern_Leaf_002.svg");
	fern.addLeaf("Fern_Leaf_003.svg");
	ofBackground(255, 255, 255);
	lastTime = ofGetElapsedTimef();
	regrowRate = 4;
}

void testApp::update(){
}

void testApp::draw(){
	ofTranslate(ofGetWidth() / 6, ofGetHeight() / 2);
	ofRotate(ofMap(mouseX, 0, ofGetWidth(), -180, 180), 0, 1, 0);
	ofRotate(ofMap(mouseY, 0, ofGetHeight(), -180, 180), 1, 0, 0);

	float curTime = fmodf(ofGetElapsedTimef(), regrowRate);

	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(32, 128, 32, 160);
	fern.draw(ofMap(curTime, 0, regrowRate / 2, 0, 1));
	ofDisableAlphaBlending();

	if(curTime < lastTime) {
		fern.randomize();
	}
	lastTime = curTime;
}

void testApp::keyPressed(int key) {
	ofSaveScreen(ofToString((int) ofRandom(1000, 9999)) + ".png");
}
