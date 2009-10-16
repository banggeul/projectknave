
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class MonsterBall : public ofxBox2dCircle {
	
public:
	
	ofImage * img;
	ofColor		color;
	MonsterBall() {
		img = NULL;
	}
	
	void draw() {
		
		ofPoint pos = getPosition();
		float	r   = getRadius();
		
		ofSetColor(color.r, color.g, color.b, color.a);
		ofFill();
		if(img) {
			ofSetRectMode(OF_RECTMODE_CENTER);
			img->draw(pos.x, pos.y, r*5, r*5);
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
		ofCircle(pos.x, pos.y, r);

	}
	
};