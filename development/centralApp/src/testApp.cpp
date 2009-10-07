#include "testApp.h"





//--------------------------------------------------------------
void testApp::setup(){

	CVM.setupVideo("testMovies/crowd-motionTest-wide-960pixelby240.mov");
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	CVM.update();
	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	CVM.draw(0,0,900,300);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

