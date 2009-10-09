#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofxSvg svg;
};
