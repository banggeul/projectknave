#include "testApp.h"





//--------------------------------------------------------------
void testApp::setup(){
	
	CVM.setupVideo("testMovies/crowd-motionTest-wide-960pixelby240.mov");
	FB.setupBuilding();
	SM.setup();
	SM.gotoScene(TREE_SCENE);
	RM.setup();
	
	drawMode = DRAW_SCENE;
	
}

//--------------------------------------------------------------
void testApp::update(){
	

	CVM.update();
	
	RM.update();
	
	SM.passInFerryBuilding(&FB);
	SM.passInPacket(CVM.packet);
	SM.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0, 0, 0);
	string info = "	FPS: "+ofToString(ofGetFrameRate());
	info += "\n		left / right key to change draw";
	info += "\n		up / down to change scenes.";
	info += "\n		Scene: "+ofToString(SM.currentScene)+"/"+ofToString(SM.numScenes);
	
	
	if (drawMode == DRAW_CV){
		CVM.draw(0,0,900,300);
		
	} else if (drawMode == DRAW_SCENE) {
		
		RM.swapInFBO();
		SM.draw();
		
		ofEnableAlphaBlending();
		ofSetColor(255,255,255, 210);
		
		ofSetColor(255, 0, 0);
		ofPoint pos = RM.getPointInPreview(mouseX, mouseY);
		ofCircle(pos.x, pos.y, 50);
		
		FB.mask.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
		
		ofSetColor(255,255,255);
		SM.drawTop();
		
		RM.swapOutFBO();
		ofSetColor(255,255,255);
		RM.drawForPreview();
		
		
	}
	else if(drawMode == DRAW_FERRY) {
		// The Ferry Building
		//FB.draw();
		FB.drawContour();
		
		info = "	FPS: "+ofToString(ofGetFrameRate());
		info += "\n	Click to draw building contour";
		info += "\n	[e] toggle on and off contours";
		info += "\n	[Space] to add new shape";
		info += "\n	[c] to clear the building";
		info += "\n	[s] to save the building to xml";
	}
	
	
	
	
	
	
	
	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	printf("c \n");
	
	SM.keyPressed(key);
	FB.keyPressed(key);
	
	switch (key){
			
		case OF_KEY_RIGHT:
			drawMode ++;
			drawMode %= 3;
			break;
		case OF_KEY_LEFT:
			drawMode --;
			if (drawMode < 0) drawMode += 3;	
			break;
			
		case OF_KEY_UP:
			SM.nextScene();
			break;
		case OF_KEY_DOWN:
			SM.prevScene();
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	SM.keyReleased(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	SM.mouseMoved(x, y);	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	SM.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	SM.mousePressed(x, y, button);
	FB.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	SM.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

