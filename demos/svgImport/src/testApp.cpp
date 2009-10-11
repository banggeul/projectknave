#include "testApp.h"

void testApp::setup(){
	svg.loadFile("patterns.svg");
	ofBackground(255, 255, 255);
}

void testApp::update(){
}

void testApp::draw(){
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	svg.draw();
	ofRotate(ofMap(mouseX, 0, ofGetWidth(), -180, 180));
	svg.draw(0, 0, mouseX, mouseY);
}
