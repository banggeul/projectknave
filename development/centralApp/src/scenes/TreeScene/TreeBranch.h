#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

class TreeBranch {
	
public:
	float alpha;
	ofxVec2f pos;
	ofxVec2f p1, p2;
	ofxVec2f p2d;
	bool	bDone;
	bool	bMadeLeave;
	TreeBranch() {
		alpha = 255;
		bDone = false;
		bMadeLeave = false;
	}
	
	void update() {
		
		
		
		p2 += (p2d - p2) / 20.0;
		
		float dis = p2.distance(p2d);
		if(dis <= 1.0) {
			bDone = true;
		}
	}
	
	void makeBranch(ofPoint goP) {
		
		p1 = goP;
		p2 = goP;
		p2d = goP;
		if((int)ofRandom(0, 2) == 0) {
			p2d.x -= ofRandom(30, 100);
		}
		else {
			p2d.x += ofRandom(30, 100);
		}
	
		p2d.y -= ofRandom(20, 100);
		
	}
	
	void draw() {
		ofFill();
		ofSetColor(255, 255, 255, alpha);
		ofCircle(pos.x, pos.y, 4);
		glLineWidth(20.0);
		ofLine(p1.x, p1.y, p2.x, p2.y);
		glLineWidth(1.0);
	}
	
	
	void drawThick() {
		/*
		glBegin(GL_QUAD_STRIP);
		for(int i=0; i<20; i++) {
			
			glVertex2f();
			glVertex2f();
			
			ofFill();
		ofSetColor(0, 250, 255);
		ofCircle(pos.x, pos.y, 4);
		
		ofLine(p1.x, p1.y, p2.x, p2.y);
		}
		glEnd();
		 */
	}
	
	
	
};