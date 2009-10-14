
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class HandBall : public ofxBox2dCircle {

public:
	HandBall() {
		
	}

	void draw() {
		
		ofPoint pos = getPosition();
		float	r   = getRadius();
		
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		ofSetColor(10, 100, 10);
		ofFill();
		ofCircle(0, 0, r);
		glPopMatrix();
		ofDisableAlphaBlending();
	}
	
};