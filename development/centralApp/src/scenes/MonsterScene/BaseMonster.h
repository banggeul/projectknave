

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"

class BaseMonster {
	
	
public:
	
	//int			id;
	ofxVec2f	pos;
	
	BaseMonster() {
	}
	
	virtual void update() {
	}
	virtual void draw() {
		
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(255, 25, 0, 100);
		
		ofCircle(pos.x, pos.y, 30);
		
		ofSetColor(255, 255, 250);
		ofDrawBitmapString(ofToString(-999), pos.x, pos.y);
		ofDisableAlphaBlending();
	}
	
};