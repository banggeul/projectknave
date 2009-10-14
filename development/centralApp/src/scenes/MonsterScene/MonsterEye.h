

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxSvg.h"


class MonsterEye {
	
	
public:
	
	float	 size, sizeD, sizeV;
	ofxVec2f pos;
	
	MonsterEye() {
		size = 0;
		sizeD = 0;
		sizeV = 0;
		
		size = ofRandom(10, 50);
	}
	
	void update() {
	}
	
	
	void draw() {
		
		ofFill();
		ofSetColor(0xffffff);
		ofCircle(pos.x, pos.y, size);
		ofSetColor(0x000000);
		ofCircle(pos.x, pos.y+10, size/3.5	);
				
	}
	
};