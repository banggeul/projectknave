/*
 *  simpleScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxDisplayList.h"

#define BUILDING_HIGHLIGHT_SIZE 8

class buildingShape {

public:

	int type;

	float animator;			// between 0 and 1
	float direction;		// -1 or 1

	vector <ofPoint >  trail;

	ofPoint centroid;
	ofRectangle boundingRect;
	int nPts;
	ofPoint * pts;

	float length;
	float energy;

	ofImage * spot;

	bool	bHasPeakedWhileTouched;
	float	peakEnergy;

	bool	bPeakedThisFrame;
	float	peakMakeStuffSick;
	bool	bAnyInsideMeFromLastFrame;
	int		nFramesSinceTrigger;

	buildingShape();
	void draw();
	void update();
	void checkInside(float x, float y, ofPoint colorMe);

	void setupList();
	bool			bSetupList;
	ofxDisplayList DL;

	ofPoint			myColor;
	ofPoint			myColorTarget;



};
