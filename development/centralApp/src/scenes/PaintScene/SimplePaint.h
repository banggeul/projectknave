
#pragma once
#include "ofMain.h"
//#include "ofxBox2d.h"


class SimplePaint  {
	
public:
	
	ofPoint pos;
	
	SimplePaint() {
		
	}
	
	void draw() {
		
		//ofPoint pos = getPosition();
		float	r   = 10;//getRadius();
		
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