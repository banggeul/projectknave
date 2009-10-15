#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(90, 90, 90);

	ofxDaito::setup("settings/daito.xml");

	building.loadImage("buildingRefrences/building.jpg");
	mask.loadImage("buildingRefrences/mask_half.png");

	XML.loadFile("settings/mainAppSettings.xml");
	bUseNetworking = XML.getValue("mainApp:useCvNetworking", 0);
	CVM.id = XML.getValue("mainApp:id", 0);

	SM.setup();
	SM.gotoScene(HAND_SCENE);

	RM.setup();

	drawMode = DRAW_SCENE;

	CVM.setupNonCV();	// this order is all wonky now.

	//MRM
	nScreens = 6;
	MRM.allocateForNScreens(nScreens, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	MRM.loadFromXml("settings/fboSettings.xml");

	float ratio = .2;

	guiIn   = ofRectangle(0, 0, OFFSCREEN_WIDTH*ratio, OFFSCREEN_HEIGHT*ratio);
	guiOut  = ofRectangle(guiIn.x + guiIn.width + 30, 40, 500, 178);
}

//--------------------------------------------------------------
void testApp::update(){


	// if we are using networking, send / recv network data:
	if (bUseNetworking == true){
		if (CVM.id == 0){
			// TODO: check the isFrameNew here...
			//if (CVM.isFrameNew()){
			CVM.sendToNetwork();
			//}
		} else if (CVM.id == 1){
			CVM.receiveFromNetwork();
		}
	}



	if (bUseNetworking){
		if (CVM.id == 0){
			CVM.update();
			//CVM.receiveFromNetwork();
		}
	} else {
		CVM.update();
	}



	//nothing in RM update at the moment.
	//RM.update();



	if ((drawMode == DRAW_SCENE)){
		SM.passInPacket(CVM.packet);
		SM.update();
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	if (bUseNetworking == true){
		if (CVM.id == 0){
			ofBackground(255,0,255);
			CVM.draw();
			return;
		}
	}



	ofSetColor(0, 0, 0);
	string info = "	FPS: "+ofToString(ofGetFrameRate());
	info += "\n		left / right key to change draw";
	info += "\n		up / down to change scenes.";
	info += "\n		Scene: "+ofToString(SM.currentScene)+"/"+ofToString(SM.numScenes);





	if (drawMode == DRAW_CV){
		CVM.draw();

	}

	else if (drawMode == DRAW_SCENE) {

		// ---- do off screen rendering
		ofSetColor(255, 255, 255, 255);
		MRM.startOffscreenDraw();

		SM.draw();
		mask.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);

		MRM.endOffscreenDraw();



		// ---- now draw the screens
		glPushMatrix();
		glTranslatef(0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		for(int i=0; i<nScreens; i++){
			MRM.drawScreen(i);
		}
		glPopMatrix();

		// ---- draw the gui utils
		ofSetColor(255, 255, 255, 255);
		MRM.drawInputDiagnostically(guiIn.x, guiIn.y, guiIn.width, guiIn.height);
		MRM.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width, guiOut.height);


		/*	RM.swapInFBO();
		 SM.draw();

		 ofEnableAlphaBlending();
		 ofSetColor(255,255,255, 210);

		 mask.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);

		 // move outside fbo can see it better
		 //ofSetColor(255,255,255);
		 //SM.drawTop();

		 RM.swapOutFBO();
		 ofSetColor(255,255,255);
		 RM.drawForPreview();
		 */
	}

	if(drawMode==DRAW_SCENE) {

		ofSetColor(255,255,255);
		//	SM.drawTop();
	}

	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	SM.keyPressed(key);

	switch (key){

		case 'f':
			ofToggleFullscreen();
			break;
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


		case 's':
			MRM.saveToXml();
			break;

		case 'r':
			MRM.reloadFromXml();
			break;

		case 'c':
			MRM.resetCoordinates();
			break;

	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	SM.keyReleased(key);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseMoved(pos.x, pos.y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseDragged(pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mouseDragged(x, y, button);
	}


	MRM.mouseDragInputPoint(guiIn, ofPoint(x, y));
	MRM.mouseDragOutputPoint(guiOut, ofPoint( x, y));


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mousePressed(pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mousePressed(x, y, button);
	}


	if( !MRM.mouseSelectInputPoint(guiIn, ofPoint(x, y)) ){
        MRM.mouseSelectOutputPoint(guiOut, ofPoint( x,  y));
    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseReleased(pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mouseReleased();
	}
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}





