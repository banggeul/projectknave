
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class MonsterBall : public ofxBox2dCircle {
	
public:
	
	ofColor		color;
	MonsterBall() {
		
	}
	
	void draw() {
		
		ofPoint pos = getPosition();
		float	r   = getRadius();
		
		ofSetColor(color.r, color.g, color.b, color.a);
		ofFill();
		ofCircle(pos.x, pos.y, r);

	}
	
};