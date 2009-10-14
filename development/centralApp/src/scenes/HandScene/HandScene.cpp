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
	box2d.setFPS(16.0);
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
	
	
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/2WindowsOnly.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Clock-Clock.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/ClockOnly.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Columns.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Roof1.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Roof2.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/StripeArea1.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/StripeArea2.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/Wall1.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/WindowFrames.bin");
	BSM.parseShapesFromFile("buildingRefrences/itoBinaryData/WindowTop_Only.bin");
	
	/*
	for (int i = nShapes; i < nShapes2; i++){

		ofxBox2dRect temp;
		temp.setup(box2d.getWorld(),  BSM.shapes[i]->boundingRect.x,  BSM.shapes[i]->boundingRect.y, BSM.shapes[i]->boundingRect.width/2, BSM.shapes[i]->boundingRect.height/2, true);
		
	}*/
	
	
	FBO.allocate(OFFSCREEN_WIDTH/5,OFFSCREEN_HEIGHT/5, false);	// fixed?
	
	
}

//--------------------------------------------------------------
void HandScene::update(){
	
	ofFill();
	FBO.swapIn();
	FBO.setupScreenForMe();
	ofEnableAlphaBlending();
	
	ofSetColor(0, 0, 0, 15);
	ofRect(0,0,640,480);
	
	
	
	
	for (int i = 0; i < BSM.shapes.size(); i++){
		
		ofSetColor(255, 0, 255, 10 * powf(BSM.shapes[i]->energy, 1.4));
		
		if (BSM.shapes[i]->trail.size() > 2){
			ofPoint pta = BSM.shapes[i]->trail[BSM.shapes[i]->trail.size()-2];
			ofPoint ptb = BSM.shapes[i]->trail[BSM.shapes[i]->trail.size()-2];
			
			ofPoint diff;
			diff.x = ptb.x - pta.x;
			diff.y = ptb.y - pta.y;
			
			for (int j = 0; j < 10; j++){
				pta.x += diff.x/9.0f;
				pta.x  += diff.x/9.0f;
				BSM.spot.draw(pta.x/5-4, pta.y/5-4, 8,8);
			}
		}
		
	}
	/*
	ofSetColor(255, 255, 255);
	ofRect(ofRandom(0,640), ofRandom(0,480), 33,33);
	*/
	FBO.swapOut();
	FBO.setupScreenForThem();
	
	
	//box2d.update();
	
	
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
		/*for(int i=0; i<box2dBuilding.size(); i++) {
			box2dBuilding[i].destroyShape();
		}	
		box2dBuilding.clear();
		*/
		float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
		float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
		
		/*
		for (int i = 0; i < packet.nBlobs; i++){
			
			box2dBuilding.push_back(ofxBox2dLine());
			box2dBuilding.back().clear();
			box2dBuilding.back().setWorld(box2d.getWorld());
			
			for (int j = packet.nPts[i]-1; j>0 ; j-=5){	
				float bx = packet.pts[i][j].x * scalex;
				float by = packet.pts[i][j].y * scaley;
				box2dBuilding.back().addPoint(bx, by);

			}
			box2dBuilding.back().createShape();

		}
		 */
	
	}
	

	BSM.update();
	
	float scalex =  (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	for (int i = 0; i < packet.nBlobs; i++){
		
		for (int j = packet.nPts[i]-1; j>0 ; j-=10){	
			float bx = packet.pts[i][j].x * scalex;
			float by = packet.pts[i][j].y * scaley;
			BSM.checkShapesForPoint(bx, by);
			
		}
		
	}
	

	lastFrameRendered = packet.frameNumber;
	
	
}

//--------------------------------------------------------------
void HandScene::draw(){
	
	
	
	ofSetColor(255, 0, 0);
	//ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	//ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);
	
	BSM.draw();
	
	
	ofEnableAlphaBlending();
	ofSetColor(50,50,50);
	glBlendFunc(GL_ONE, GL_ONE);
	FBO.draw(0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofEnableAlphaBlending();
	
	
	//ferryBuilding.drawContour();
	//ferryBuilding.drawInfo();
	
	
	
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
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < packet.nPts[i]; j++){	
			glVertex2f(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		glEnd();
		
		ofSetColor(255, 255, 255, 85);
		/*ofBeginShape();
		for (int j = 1; j < packet.nPts[i]; j+=3){	
			ofCurveVertex(packet.pts[i][j].x * scalex, packet.pts[i][j].y * scaley);
		}
		ofEndShape(true);*/
	}
	
	

	
	
	// draw some new stuff. 
	for(int i=0; i<balls.size(); i++) {
		//balls[i].draw();	
	}
	
	for(int i=0; i<box2dHandShape.size(); i++) {
		//box2dHandShape[i].draw();	
	}
	
	
	//floor.draw();
	//box2d.draw();
}

//--------------------------------------------------------------
void HandScene::drawTop(){
	
	
	/*
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
	*/
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












