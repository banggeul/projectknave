/*
 *  simpleScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"
#include "ofxCvGrayscaleAdvanced.h"

class HandScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	void drawTop();
	
	int tempC;
	
	int	handImgW, handImgH;
	
	
	ofxCvGrayscaleAdvanced				handImage;
	ofxCvGrayscaleAdvanced				handImageTemporallyBlurred;
	ofxCvGrayscaleAdvanced				handImageTemporallyBlurredInvert;

	
	int lastFrameRendered;
	
	
	/*
	 
	 for finding building shapes ! 
	 
	 */
	
	ofImage				roof;
	int width, height;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvContourFinder 	contourFinder;
	int 				threshold;
	bool				bLearnBakground;
	
};