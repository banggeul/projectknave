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
	tempC = (int)ofRandom(0, 255);
	
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

void HandScene::update(){
	
	
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
	}
	

	lastFrameRendered = packet.frameNumber;
	 
	
	// draw some new stuff. 
	
	
}

void HandScene::draw(){
	ofSetColor(255, 0, 0);
	ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);
	
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
	
}

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