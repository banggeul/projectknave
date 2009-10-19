#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(90, 90, 90);
	drawMode = DRAW_SCENE;

	effectiveWidth = OFFSCREEN_WIDTH * OFFSCREEN_SCALE;
	effectiveHeight = OFFSCREEN_HEIGHT * OFFSCREEN_SCALE;

	// -------- The images for the building
	building.loadImage("buildingRefrences/building.jpg");
	mask.loadImage("buildingRefrences/mask_half.png");

	// -------- App settings

	XML.loadFile("settings/mainAppSettings.xml");

	int useDaito = XML.getValue("mainApp:useDaito", 1);
	if(useDaito)
		ofxDaito::setup("settings/daito.xml");

	bUseNetworking = XML.getValue("mainApp:useCvNetworking", 0);
	CVM.id = XML.getValue("mainApp:id", 0);

	// -------- Scenes
	SM.setup(CVM);
	RM.setup();
	CVM.setupNonCV();	// this order is all wonky now.

	// give every scene the stitched image reference
	for(int i = 0; i < SM.numScenes; i++)
		SM.scenes[i]->stitchedImage = &(CVM.VideoFrame);

	// Mega Render Manager
	nScreens	= 6;		// <--- if you just want to work on your mac set to one screen
	MRM.allocateForNScreens(nScreens, effectiveWidth, effectiveHeight);
	//MRM.loadFromXml("settings/fboSettingsLive.xml");
	MRM.loadFromXml("settings/fboSettings.xml");
	float ratio = .2;

	guiIn   = ofRectangle(0, 0, OFFSCREEN_WIDTH*ratio, OFFSCREEN_HEIGHT*ratio);
	guiOut  = ofRectangle(guiIn.x + guiIn.width + 30, 40, 500, 178);

	bDrawInDebugWay = true;
	showInfo = true;


	SM.gotoScene(TREE_SCENE);
	//SM.gotoScene(MONSTER_SCENE);
}

//--------------------------------------------------------------
void testApp::update() {


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


	int sendDaitoPixelDataRate = 5;

	if (ofGetFrameNum() % sendDaitoPixelDataRate == 0){
		ofxOscMessage msg;
		msg.setAddress("/cvData");							//	/cvData
		msg.addStringArg("pctWhitePixels");					//	"pctWhitePixels"
		msg.addFloatArg(CVM.packet->pctWhitePixels);		//	float
		msg.addStringArg("pctMovingPixels");				//	"pctPixelsMoving"
		msg.addFloatArg(CVM.packet->pctPixelsMoving);		//	float
		ofxDaito::sendCustom(msg);
	}



}

//--------------------------------------------------------------
void testApp::draw() {

	// ----------------------------------
	//  --------  Vision ----------------
	// ----------------------------------
	if (drawMode == DRAW_CV){
		CVM.draw();
	}

	// ----------------------------------
	//  -------- Scenese ----------------
	// ----------------------------------
	else if (drawMode == DRAW_SCENE) {

		// ---- do off screen rendering
		ofSetColor(255, 255, 255, 255);
		MRM.startOffscreenDraw();

		ofPushStyle();
		ofPushMatrix();
		glScalef(OFFSCREEN_SCALE, OFFSCREEN_SCALE, 1.0f);
		SM.draw();
		ofPopMatrix();
		ofPopStyle();

		ofEnableAlphaBlending();
		mask.draw(0, 0, effectiveWidth, effectiveHeight);
		ofDisableAlphaBlending();

		MRM.endOffscreenDraw();


		if (!bDrawInDebugWay){
			// ---- now draw the screens
			glPushMatrix();
			glTranslatef(0, 0, 0);
			ofSetColor(255, 255, 255, 255);
			for(int i=0; i<nScreens; i++){
				MRM.drawScreen(i);
			}
			glPopMatrix();

			// ---- draw the gui utils
			if(showInfo) {
				ofSetColor(255, 255, 255, 255);
				MRM.drawInputDiagnostically(guiIn.x, guiIn.y, guiIn.width, guiIn.height);
				MRM.drawOutputDiagnostically(guiOut.x, guiOut.y, guiOut.width, guiOut.height);
			}
		} else {
			ofSetColor(255, 255, 255, 255);
			if(effectiveWidth < ofGetWidth() && effectiveHeight < ofGetHeight()) {
				MRM.myOffscreenTexture.draw(0, ofGetHeight() - effectiveHeight, effectiveWidth, effectiveHeight);
			} else {
				float ratio = (float) effectiveHeight / (float) effectiveWidth;
				float scaledWidth = ofGetWidth();
				float scaledHeight = ratio * scaledWidth;
				MRM.myOffscreenTexture.draw(0, ofGetHeight() - scaledHeight, scaledWidth, scaledHeight);
			}

			ofSetColor(255, 255, 255);
			SM.drawTop();
		}
	}

	if(showInfo) {
		string info = "	FPS: " + ofToString(ofGetFrameRate());
		info += "\n		left / right key to change draw";
		info += "\n		up / down to change scenes";
		info += "\n   f to toggle fullscreen";
		info += "\n   m multiscreen with fbo";
		info += "\n		Scene: "+ofToString(SM.currentScene)+"/"+ofToString(SM.numScenes);
		ofSetColor(255, 255, 0);
		ofDrawBitmapString(info, 20, 20);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	SM.keyPressed(key);

	switch (key){
		case 'o':			ofxDaito::toggleOutput();		case 'f':
			ofToggleFullscreen();
			break;
		case 'm':
			bDrawInDebugWay = !bDrawInDebugWay;
			if(bDrawInDebugWay)
				ofShowCursor();
			else
				ofHideCursor();
			break;
		case 'i':
			showInfo = !showInfo;
			break;

		case OF_KEY_RIGHT:
			drawMode ++;
			drawMode %= 2;
			break;
		case OF_KEY_LEFT:
			drawMode --;
			if (drawMode < 0) drawMode += 2;
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
void testApp::mouseMoved(int x, int y ) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseMoved(x, y, pos.x, pos.y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseDragged(x, y, pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mouseDragged(x, y, button);
	}


	MRM.mouseDragInputPoint(guiIn, ofPoint(x, y));
	MRM.mouseDragOutputPoint(guiOut, ofPoint( x, y));


}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mousePressed(x, y, pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mousePressed(x, y, button);
	}

	/*
	 if( !MRM.mouseSelectInputPoint(guiIn, ofPoint(x, y)) ){
	 MRM.mouseSelectOutputPoint(guiOut, ofPoint( x,  y));
	 }
	 */
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseReleased(x, y, pos.x, pos.y, button);

	if (drawMode == DRAW_CV || bUseNetworking){
		CVM.mouseReleased();
	}
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}





