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
#include "particleManager.h"
#include "ofxCvGrayscaleAdvanced.h"


typedef struct {
	ofPoint		center;
	ofPoint		prevCenter;
	ofPoint		vel;
	float		avgVel;
	float		energy;
	int			id;
	int			whoThisFrame;
	bool		bFoundThisFrame;
	int			nFramesFound;
}	starObject;



class StarScene : public baseScene {

public:

	void setup();
	void update();
	void draw();
	void drawTop();


	void track();

	int tempC;

	void announceChange(int change);
	void keyPressed(int key);

	 particleManager PM;
		ofImage		temp;
		ofImage skyImage;

	bool					bAllocatedYet;
	ofxCvGrayscaleImage		videoGray;
	ofxCvGrayscaleImage		videoGraySmoothed;
	ofxCvGrayscaleAdvanced		videoGrayTemporallySmoothed;
	ofxCvGrayscaleAdvanced		videoGrayTemporallySmoothed2;


	ofxCvContourFinder		contours;

	vector < starObject > SOBJ;

	bool	bFinal;
};


/*



 temp.loadImage("images/logoHalf.png");
 temp.setImageType(OF_IMAGE_GRAYSCALE);
 PM.findLockTarget(temp.getPixels(), temp.width, temp.height);


 */
