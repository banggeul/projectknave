


#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "TreeConstants.h"

class TreeLeaf {
	
public:
	
	ofPoint		pos;
	ofColor		color;
	bool		bDone;
	ofImage * img;
	float r;
	
	
	
	TreeLeaf() {
		img = NULL;
		bDone = false;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 0;
	}
	
	void update() {
		color.a += 20.0;
		if(color.a >= 255) {
			color.a = 255;
			bDone = true;
		}
		
	}
	
	void draw() {
		
		
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