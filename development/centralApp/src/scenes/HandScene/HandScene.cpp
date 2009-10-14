/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "HandScene.h"


void HandScene::setup(){
	
	// box2d
	box2d.init();
	box2d.setGravity(0, 30);
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
	printf("box2d allocated\n");
	
	// the ferry building contour
	ferryBuilding.setupBuilding("buildingRefrences/buidlingFiles/handsBuildingContour.xml");
	// hit space to make a ferry building contour -- needs work
	
	
	// make a big ball
	HandBall bub;
	bub.setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
	bub.setup(box2d.getWorld(), OFFSCREEN_WIDTH/2, 120, 50);
	balls.push_back(bub);
	
	floor.setup(box2d.getWorld(), 0, OFFSCREEN_HEIGHT-20, OFFSCREEN_WIDTH, 20, true);
	
	// ----------
	
	
	handImgW = 640;
	handImgH = 480;
	
	
	handImage.setUseTexture(false);
	handImage.allocate(handImgW, handImgH);
	
	handImageTemporallyBlurred.setUseTexture(false);
	handImageTemporallyBlurred.allocate(handImgW, handImgH);
	handImageTemporallyBlurredInvert.allocate(handImgW, handImgH);
	
	handImage.preAllocate(MAX_BLOB_LENGTH);
	lastFrameRendered = -1;
	
	//------------------------------------------- shape finding
	roof.loadImage("buildingRefrences/itoMasks/roof.png");
	roof.setImageType(OF_IMAGE_GRAYSCALE);
	width = roof.width;
	height = roof.height;
	printf("loaded roof, width %i height %i \n", width, height);
	grayImage.allocate(width,height);
	grayImage.setFromPixels(roof.getPixels(), width, height);
	contourFinder.findContours(grayImage, 20, width*height, 100, false);	// find holes
	
	
	
	
	
	
}

//--------------------------------------------------------------
void HandScene::update(){
	
	box2d.update();
	
	
	if (lastFrameRendered !=  packet.frameNumber){
		
		handImage.set(0);
		
		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (!packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 100);
			}
		}
		
		for (int i = 0; i < packet.nBlobs; i++){
			int nPts = packet.nPts[i];
			if (packet.bAmInner[i]){
				handImage.drawBlobIntoMeWithPreallocatedPts(packet.pts[i], nPts, 0);
			}
		}
		
		handImage.blur();
		
		handImageTemporallyBlurred -= 50;
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred.blur();
		handImageTemporallyBlurred += handImage;
		
		handImageTemporallyBlurredInvert = handImageTemporallyBlurred;
		handImageTemporallyBlurredInvert.invert();
		
		
		
		// clear out the shape
		for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].destroyShape();
		}	
		box2dBuilding.clear();
		
		float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
		float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
		
		for (int i = 0; i < packet.nBlobs; i++){
			
			box2dBuilding.push_back(ofxBox2dLine());
			box2dBuilding.back().clear();
			box2dBuilding.back().setWorld(box2d.getWorld());
			
			for (int j = packet.nPts[i]-1; j>0 ; j--){	
				float bx = packet.pts[i][j].x * scalex;
				float by = packet.pts[i][j].y * scaley;
				box2dBuilding.back().addPoint(bx, by);

			}
			box2dBuilding.back().createShape();

		}
	
	}
	
	
	lastFrameRendered = packet.frameNumber;
	
	
}

//--------------------------------------------------------------
void HandScene::draw(){
	
	
	ofSetColor(255, 0, 0);
	ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);
	
	
	
	
	ferryBuilding.drawContour();
	ferryBuilding.drawInfo();
	
	
	
	ofEnableAlphaBlending();
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	ofSetColor(255, 255, 255);
	handImageTemporallyBlurredInvert.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	
	ofEnableAlphaBlending();
	
	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	for (int i = 0; i < packet.nBlobs; i++){
		
		/*glBegin(GL_LINE_LOOP);
		 for (int j = 0; j < packet.nPts[i]; j++){	
		 glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		 }
		 glEnd();
		 */
		
		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			ofVertex(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		ofEndShape(true);
		
		ofSetColor(255, 255, 255, 85);
		ofBeginShape();
		for (int j = 1; j < packet.nPts[i]; j+=3){	
			ofCurveVertex(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		ofEndShape(true);
	}
	
	
	bool bOn[contourFinder.nBlobs];
	for (int i = 0; i < contourFinder.nBlobs; i++){
		bOn[i] = false;
	}
	
	for (int k = 0; k < contourFinder.nBlobs; k++){
		
		// let's pass bounding rectangles thrus this is a pain! 
		for (int i = 0; i < packet.nBlobs; i++){
			for (int j = 0; j < packet.nPts[i]; j+=5){	
				
				float x = packet.pts[i][j].x * scalex;
				float y = packet.pts[i][j].y * scaley;
				
				if ((x > contourFinder.blobs[k].boundingRect.x) && (x < (contourFinder.blobs[k].boundingRect.x + contourFinder.blobs[k].boundingRect.width)) &&
					(y > contourFinder.blobs[k].boundingRect.y) && (y < (contourFinder.blobs[k].boundingRect.y + contourFinder.blobs[k].boundingRect.height))){
					
					
					bOn[k] = true;
					
				}
				
			}
		}
	}
	
	for (int i = 0; i < contourFinder.nBlobs; i++){
		if (bOn[i] == true){
			contourFinder.blobs[i].draw();
		}
	}
	
	
	
	// draw some new stuff. 
	for(int i=0; i<balls.size(); i++) {
		balls[i].draw();	
	}
	
	for(int i=0; i<box2dHandShape.size(); i++) {
		box2dHandShape[i].draw();	
	}
	
	
	floor.draw();
	//box2d.draw();
}

//--------------------------------------------------------------
void HandScene::drawTop(){
	for (int i = 0; i < packet.nBlobs; i++){
		ofFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			ofVertex(packet.pts[i][j].x, packet.pts[i][j].y);
		}
		ofEndShape(true);
	}
	
	ofSetColor(255, 255, 255);
	//handImageTemporallyBlurred.draw(0,400);
	
}


//--------------------------------------------------------------
void HandScene::keyPressed(int key) {
	ferryBuilding.keyPressed(key);
	
	if(key == ' ') {
		
		for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].destroyShape();
		}	
		box2dBuilding.clear();
		
		// build the building for box2d
		for(int i=0; i<ferryBuilding.shapes.size(); i++) {
			box2dBuilding.push_back(ofxBox2dLine());
			box2dBuilding.back().clear();
			box2dBuilding.back().setWorld(box2d.getWorld());
			
			for(int j=0; j<ferryBuilding.shapes[i].pnts.size(); j++) {
				float bx = ferryBuilding.shapes[i].pnts[j].x;
				float by = ferryBuilding.shapes[i].pnts[j].y;
				box2dBuilding.back().addPoint(bx, by);
			}
			box2dBuilding.back().createShape();
		}
	}
}

//--------------------------------------------------------------
void HandScene::mousePressed(int x, int y, int button) {
	
	ferryBuilding.mousePressed(x, y, button);
	
	HandBall bub;
	bub.setPhysics(3.0, 0.53, 0.1); // mass - bounce - friction
	bub.setup(box2d.getWorld(), x, y, 20);
	balls.push_back(bub);
	
}












