
#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"



class Stem {

public:
	
	int stemLength;
	
	// ------------------------------------------
	Stem() {
		stemLength = (int)ofRandom(10, 50);
	}
	
	// ------------------------------------------
	void update() {
		
	}
	
	// ------------------------------------------
	void draw() {
	}
	
};