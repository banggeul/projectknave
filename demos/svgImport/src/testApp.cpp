#include "testApp.h"

void testApp::setup(){
	svg.loadFile("patterns.svg");
	ofBackground(255, 255, 255);
}

void testApp::update(){
}

void testApp::draw(){
	ofTranslate(svg.getAnchor().x, svg.getAnchor().y);
	svg.draw();
	ofRotate(ofMap(mouseX, 0, ofGetWidth(), -180, 180));
	svg.draw(0, 0, mouseX, mouseY);
}
