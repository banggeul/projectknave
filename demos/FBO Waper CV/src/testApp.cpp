#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(10, 10, 10);
	ofSetVerticalSync(true);
	
	cw = 320;
	ch = 240;
	camera.initGrabber(cw, ch);
	
	
	
    rm.allocateForNScreens(2, cw*2, ch);
    rm.loadFromXml("fboSettings.xml");
	
    guiIn   = ofRectangle(0, 0, cw*2, ch);
	//  guiOut  = ofRectangle(guiIn.x+guiIn.width+30, 0, cw*2, ch);
	
	dualImage.allocate(cw*2, ch, OF_IMAGE_COLOR);
	color.allocate(cw*2, ch);
	gray.allocate(cw*2, ch);
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	camera.update();
	
	if(camera.isFrameNew()) {
		
		ofSetColor(0xffffff);
		rm.startOffscreenDraw();
		
		// -------------- draw the camera image
		ofSetColor(0xffffff);
		camera.draw(0, 0);
		camera.draw(cw, 0);
		rm.endOffscreenDraw();
		
		
		// -------------- draw the sceamed image
		glPushMatrix();
		glTranslatef(0, ch+30, 0);
		ofSetColor(0xffffff);
		rm.drawScreen(0);
		rm.drawScreen(1);
		
		// opencv grabing
		dualImage.grabScreen(0, ch+30, cw*2, ch);
		color.setFromPixels(dualImage.getPixels(), cw*2, ch);
		gray = color;
		gray.threshold(20, true);
		contour.findContours(gray, 30, (cw*ch)/3, 10, false);
		glPopMatrix();
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	
	// guis
	ofSetColor(0xffffff);
	rm.drawInputDiagnostically(guiIn.x, guiIn.y, guiIn.width, guiIn.height);
	
	// draw opencv
	glPushMatrix();
	glTranslatef(0, ch+30, 0);
	
	ofSetColor(0xffffff);
	color.draw(0, 0);
	
	ofSetColor(0xffffff);
	gray.draw((cw*2)+30, 0);
	
	contour.draw((cw*2)+30, 0);
	ofSetColor(100, 100, 100);
	string info = "	FPS: "+ofToString(ofGetFrameRate());
	ofDrawBitmapString(info, 20, ch+20);
	
	glPopMatrix();
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
    if( key == 's'){
        rm.saveToXml();
    }
	
    if( key == 'r' ){
        rm.reloadFromXml();
    }
	
    if(key == 'c'){
        rm.resetCoordinates();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    rm.mouseDragInputPoint(guiIn, ofPoint(x, y));
    rm.mouseDragOutputPoint(guiOut, ofPoint( x, y));
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if( !rm.mouseSelectInputPoint(guiIn, ofPoint(x, y)) ){
        rm.mouseSelectOutputPoint(guiOut, ofPoint( x,  y));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
	
}
