/*
 *  simpleScene.cpp
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MonsterScene.h"
#include "ofxVectorMath.h"

void MonsterScene::setup(){
	tempC = (int)ofRandom(0, 255);
	cw = 640;
	ch = 480;
}

void MonsterScene::update(){
	
}

void MonsterScene::draw(){
	
	ofSetColor(255, 0, 0);
	ofLine (0,0,OFFSCREEN_WIDTH, OFFSCREEN_HEIGHT);
	ofLine (0,OFFSCREEN_HEIGHT,OFFSCREEN_WIDTH, 0);
	
	
	
	float scalex = (float)OFFSCREEN_WIDTH / (float)packet.width;
	float scaley = (float)OFFSCREEN_HEIGHT / (float)packet.height;
	
	
	
	ofEnableAlphaBlending();
	
	// ---------- v1
	glPushMatrix();
	glTranslatef(0, packet.height, 0);
	
	ofFill();
	ofSetColor(255, 0, 0, 20);
	ofRect(0, 0, packet.width, packet.height);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			ofVertex(packet.pts[i][j].x, packet.pts[i][j].y);
		}
		ofEndShape(true);
	}
	glPopMatrix();

	
	// ---------- v2
	glPushMatrix();
	glTranslatef(packet.width, packet.height, 0);
	
	ofFill();
	ofSetColor(255, 0, 0, 20);
	ofRect(0, 0, packet.width, packet.height);
	
	for(int i=0; i<packet.nBlobs; i++) {
		ofSetColor(255, 255, 255, 100);
		ofNoFill();
		ofEnableSmoothing();
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			ofVertex(packet.pts[i][j].x, packet.pts[i][j].y);
		}
		ofEndShape(true);
	}
	glPopMatrix();
	
	ofDisableAlphaBlending();
	
	
	
	return;
	
	ofFill();
	ofSetColor(tempC, tempC, 255);
	ofRect(0,0,320,240);
	
	
	for (int i = 0; i < packet.nBlobs; i++){
		ofNoFill();
		ofSetColor(255, 0, 0);
		ofBeginShape();
		for (int j = 0; j < packet.nPts[i]; j++){	
			
			float x = packet.pts[i][j].x;
			float y = packet.pts[i][j].y;
			
			ofVertex(x, y);
		}
		ofEndShape(true);
	}
	
	
	
	
	for (int i = 0; i < packet.nBlobs; i++){
		ofSetColor(255, 0, 255);
		for (int j = 0; j < packet.nPts[i]; j++){	
			
			float x = (packet.pts[i][j].x / cw) * (float)OFFSCREEN_WIDTH;
			float y = (packet.pts[i][j].y / ch) * (float)OFFSCREEN_HEIGHT;
			
			if(j > 0 && packet.pts[i][j].y < 220.0) {
				
				ofxVec2f p1, p2;
				p1.x = (packet.pts[i][j].x / cw) * (float)OFFSCREEN_WIDTH;
				p1.y = (packet.pts[i][j].y / ch) * (float)OFFSCREEN_HEIGHT;
				p2.x = (packet.pts[i][j-1].x / cw) * (float)OFFSCREEN_WIDTH;
				p2.y = (packet.pts[i][j-1].y / ch) * (float)OFFSCREEN_HEIGHT;
				
				ofxVec2f mid = p1 - p2;
				mid.normalize();
				
				ofxVec2f norml(-mid.y, mid.x);
				ofxVec2f p3 = p1 + (norml * 30.0);
				
				ofLine(x, y, p3.x, p3.y);
			}
		}
	}
}