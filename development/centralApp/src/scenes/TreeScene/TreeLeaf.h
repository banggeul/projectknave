


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
	float alpha;
	
	
	TreeLeaf() {
		img = NULL;
		bDone = false;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 0;
		alpha = 0;
		r = ofRandom(10, 20);
	}
	
	void update() {
		if(!bDone) alpha += 10.0;
		if(alpha >= 255) {
			alpha = 255;
			bDone = true;
		}
		
	}
	
	void draw() {
		
		ofEnableAlphaBlending();
		ofSetColor(color.r, color.g, color.b, alpha);
		ofFill();
		if(img) {
			ofSetRectMode(OF_RECTMODE_CENTER);
			img->draw(pos.x, pos.y, r*2, r*2);
			ofSetRectMode(OF_RECTMODE_CORNER);
		}
		ofEllipse(pos.x, pos.y, r, r);
		ofDisableAlphaBlending();
		
	}
	
	
	
};