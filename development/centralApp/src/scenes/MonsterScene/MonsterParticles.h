




#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2dCircle.h"

#define MAX_POP_BITS 25


class MonsterParticles : public ofxBox2dCircle {
	
	
public:
	
	bool		bGoPop;
	bool		bDead;
	ofColor		clr;
	
	int			numPopBits;
	ofPoint		popA[MAX_POP_BITS];
	ofPoint		popB[MAX_POP_BITS];
	float		popR[MAX_POP_BITS];
	
	float		deadDate, age;
	float		r;
	
	// ----------------------------------------
	MonsterParticles() {
		
		bDead  = true;
		bGoPop = false;
		
		clr.r = ofRandom(0, 255);
		clr.g = ofRandom(0, 255);
		clr.b = ofRandom(0, 255);
		clr.a = 255;
		
		age			= 0;
		deadDate	= ofRandom(30, 120);
		numPopBits  = (int)ofRandom(10, 20);
		
		enableGravity(false);
	}
	

	// ----------------------------------------
	void init() {
		bDead = false;
		clr.r = ofRandom(0, 255);
		clr.g = ofRandom(0, 255);
		clr.b = ofRandom(0, 255);	
	}
	
	// ----------------------------------------
	void update() {
		
		
		age ++;
		if(age >= deadDate && !bGoPop) {
			bGoPop = true;	
			
			for(int i=0; i<numPopBits; i++) {
				float n = (float)i/ (float)(numPopBits-1);
				popA[i].x  = cos(n * TWO_PI) * (r+10);
				popA[i].y  = sin(n * TWO_PI) * (r+10);
				
				popB[i].x  = cos(n * TWO_PI) * (r+ofRandom(12, 45));
				popB[i].y  = sin(n * TWO_PI) * (r+ofRandom(12, 45));
			
				popR[i] = ofRandom(3.0, 10.0);
			}
		}
		
		
		// time to pop !
		if(bGoPop) {
			
			r +=1.7;
			clr.a -= 20.0;
			if(r <= 2 || clr.a <= 0.0) {
				bDead = true;
			}
			
			for(int i=0; i<numPopBits; i++) {
				popA[i] += (popB[i]-popA[i]) / popR[i];
			}
		}
		
		
		// monster farts fly up duhh
		addForce(0, -10);
		
		
		
	}
	
	// ----------------------------------------
	void draw() {
		
		if(bDead) return;
		
		//ofEnableAlphaBlending();
		ofPoint		pos = getPosition();
		
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		
		if(!bGoPop) r   = getRadius();
		
		
		// the spary
		if(bGoPop) {
			ofSetColor(clr.r, clr.g, clr.b, clr.a);
			for(int i=0; i<numPopBits; i++) {
				ofBeginShape();
				for(int j=0; j<4; j++) {
					ofVertex(popA[i].x+ofRandom(-3, 3), popA[i].y+ofRandom(-3, 3));
					ofVertex(popA[i].x+ofRandom(-3, 3), popA[i].y+ofRandom(-3, 3));
					ofVertex(popA[i].x+ofRandom(-3, 3), popA[i].y+ofRandom(-3, 3));
				}
				ofEndShape();
			}
		}
		
		
		
		
		
		ofSetColor(clr.r, clr.g, clr.b, clr.a);
		ofFill();
		ofCircle(0, 0, r);
		glPopMatrix();
		//ofDisableAlphaBlending();
	}
};