
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class MonsterBall : public ofxBox2dCircle {
	
public:
	MonsterBall() {
		
	}
	
	void draw() {
		
		ofPoint pos = getPosition();
		float	r   = getRadius();
		
		ofCircle(pos.x, pos.y, r);
		return;
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