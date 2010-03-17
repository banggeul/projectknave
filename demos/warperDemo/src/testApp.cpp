#include "testApp.h"

void testApp::setup() {
	ofSetFrameRate(30);
	ofBackground(255, 255, 255);

	warper.setup("ferry-building.jpg", 10, 48);
}

void testApp::update() {
	warper.addAttraction(
		ofMap(mouseX, 0, ofGetWidth(), 0,warper.getWidth()),
		ofMap(mouseY, 0, ofGetHeight(), 0, warper.getHeight()),
		-1, 20);
	warper.update();
}

void testApp::draw(){
	warper.draw(0, 0, ofGetWidth(), ofGetHeight());
}
