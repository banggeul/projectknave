#include "testApp.h"





//--------------------------------------------------------------
void testApp::setup(){
	
	
	//ofDisableArbTex();
	
	XML.loadFile("settings/mainAppSettings.xml");
	bUseNetworking = XML.getValue("mainApp:useCvNetworking", 0);	
	CVM.id = XML.getValue("mainApp:id", 0);
	
	
	
	FB.setupBuilding();
	SM.setup();
	SM.gotoScene(MONSTER_SCENE);
	RM.setup();
	
	drawMode = DRAW_SCENE;

	// box2d
	bEnableBox2d = false;
	box2d.init();
	box2d.setGravity(0, 10);
	//box2d.createFloor();
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	

		
	CVM.setupNonCV();	// this order is all wonky now. 

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
		SM.passInFerryBuilding(&FB);
		SM.passInPacket(CVM.packet);
		SM.update();
	}

	if(bEnableBox2d) {
		
		box2d.update();
		
		// kill if bellow ground
		for(int i=0; i<circles.size(); i++) {
			ofPoint pos = circles[i].getPosition();
			if(pos.y > ofGetHeight()+400) {
				circles[i].destroyShape();	
				circles.erase(circles.begin() + i);
			}
		}
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
		
	} else if (drawMode == DRAW_SCENE) {
		
		RM.swapInFBO();
		SM.draw();
		
		ofEnableAlphaBlending();
		ofSetColor(255,255,255, 210);
		
		ofSetColor(255, 0, 0);
		ofPoint pos = RM.getPointInPreview(mouseX, mouseY);
		ofFill();
		ofCircle(pos.x, pos.y, 50);
		
		FB.mask.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
		
		ofSetColor(255,255,255);
		SM.drawTop();
		
		RM.swapOutFBO();
		ofSetColor(255,255,255);
		RM.drawForPreview();
		
		
	}
	else if(drawMode == DRAW_FERRY) {
		
		RM.swapInFBO();
		ofEnableAlphaBlending();
		ofSetColor(255,255,255, 210);
		
		// box2d stuff temp
		if(bEnableBox2d) {
			//box2d.draw();
			for(int i=0; i<box2dBuilding.size(); i++) {
				box2dBuilding[i].draw();
			}
			for(int i=0; i<circles.size(); i++) {
				ofSetColor(0x2388ff);
				circles[i].draw();
			}
		}	
		
		
		//FB.mask.draw(0, 0, OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
		
		// The Ferry Building
		FB.drawContour();

		RM.swapOutFBO();
		ofSetColor(255,255,255);
		RM.drawForPreview();
		
		info = "";
		FB.drawInfo();
	}

	ofDrawBitmapString(info, 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	SM.keyPressed(key);
	FB.keyPressed(key);
	
	switch (key){
			
		case 'b':
			bEnableBox2d = !bEnableBox2d;
			for(int i=0; i<FB.shapes.size(); i++) {
				
				box2dBuilding.push_back(ofxBox2dLine());
				box2dBuilding.back().clear();
				box2dBuilding.back().setWorld(box2d.getWorld());
				
				for(int j=0; j<FB.shapes[i].pnts.size(); j++) {
					float bx = FB.shapes[i].pnts[j].x;
					float by = FB.shapes[i].pnts[j].y;
					ofPoint pos = RM.getPointInPreview(bx, by);
					box2dBuilding.back().addPoint(bx, by);
				}
				box2dBuilding.back().createShape();
				
			}
			break;
			
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
			
			// set building box2d contour
		case '1':
			for(int i=0; i<FB.shapes.size(); i++) {
				
				box2dBuilding.push_back(ofxBox2dLine());
				box2dBuilding.back().clear();
				box2dBuilding.back().setWorld(box2d.getWorld());
				
				for(int j=0; j<FB.shapes[i].pnts.size(); j++) {
					float bx = FB.shapes[i].pnts[j].x;
					float by = FB.shapes[i].pnts[j].y;
					ofPoint pos = RM.getPointInPreview(bx, by);
					box2dBuilding.back().addPoint(bx, by);
				}
				box2dBuilding.back().createShape();
				
			}
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
	
	if (drawMode == DRAW_CV){
		CVM.mouseDragged(x, y, button);
	}
	
	
	if(bEnableBox2d) {
		float r = ofRandom(2, 27);
		ofxBox2dCircle bub;
		bub.setPhysics(3.0, 0.53, 0.1);
		bub.setup(box2d.getWorld(), pos.x, pos.y, r);
		circles.push_back(bub);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mousePressed(pos.x, pos.y, button);
	FB.mousePressed(pos.x, pos.y, button);
	
	if (drawMode == DRAW_CV){
		CVM.mousePressed(x, y, button);
	}
	
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	ofPoint pos = RM.getPointInPreview(x, y);
	SM.mouseReleased(pos.x, pos.y, button);
	
	if (drawMode == DRAW_CV){
		CVM.mouseReleased();
	}
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

