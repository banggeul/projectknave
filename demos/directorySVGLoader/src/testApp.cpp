#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	eyes.load("monsters/eyes");
	confetti.load("monsters/confetti");
	butterflies.load("butterflies");
}

//--------------------------------------------------------------
void testApp::update(){
	ofSeedRandom();
}


//--------------------------------------------------------------
void testApp::draw(){
	if(eyes.isLoaded()){
		eyes.getRandomSvg().draw(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
	}
	if(confetti.isLoaded()){
		confetti.getRandomSvg().draw(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
	}
	if(butterflies.isLoaded()){
		butterflies.getRandomSvg().draw(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
	}
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

