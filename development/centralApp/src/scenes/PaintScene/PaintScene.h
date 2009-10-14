/*
 *  PaintScene.h
 *  openFrameworks
 *
 *  Created by Joel Gethin Lewis on 08/10/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once;

#include "ofMain.h"
#include "baseScene.h"

#define NUM_GROUND_PNTS 20

class PaintScene : public baseScene {
	
public: 
	
	ofxVec2f groundPnts[NUM_GROUND_PNTS];
	
	
	// ------------------------------------------------
		
	
	// box2d building lines
	ofxBox2d					box2d;

	vector <ofxBox2dLine>	buildingBoundsShape;

	
	// ------------------------------------------------
	void setup();
	void update();
	void draw();
	
	
};