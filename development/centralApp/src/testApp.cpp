#include "testApp.h"





//--------------------------------------------------------------
void testApp::setup(){

	CVM.setupVideo("testMovies/crowd-motionTest-wide-960pixelby240.mov");
	SM.setup();
	
	
	drawMode = DRAW_SCENE;
}

//--------------------------------------------------------------
void testApp::update(){
	
	CVM.update();
	
	SM.passInPacket(CVM.packet);
	SM.update();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if (drawMode == DRAW_CV){
		CVM.draw(0,0,900,300);
	} else if (drawMode == DRAW_SCENE) {
		SM.draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
			
		case OF_KEY_RIGHT:
			drawMode ++;
			drawMode %= 2;
			break;
		case OF_KEY_LEFT:
			drawMode --;
			if (drawMode < 0) drawMode += 2;	
			break;
	}
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

