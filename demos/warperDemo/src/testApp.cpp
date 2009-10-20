#include "testApp.h"

void testApp::setup() {
	ofSetFrameRate(30);
	ofBackground(255, 255, 255);

	warper.setup("ferry-building.jpg", 30, 50);
}

void testApp::update() {
	warper.addAttraction(mouseX, mouseY, ofGetWidth(), ofGetHeight()	);
	warper.update();
}

void testApp::draw(){
	warper.draw(0, 0, ofGetWidth(), ofGetHeight());
}
