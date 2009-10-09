#include "testApp.h"

void testApp::setup(){
	svg.loadFile("patterns.svg");
	ofBackground(255, 255, 255);
}

void testApp::update(){
}

void testApp::draw(){
	ofTranslate(svg.getAnchor().x, svg.getAnchor().y);
	ofRotate(ofMap(mouseX, 0, ofGetWidth(), -180, 180));
	svg.draw();
}
