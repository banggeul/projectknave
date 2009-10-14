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


class buildingShape {
	
public: 

	
	float animator;			// between 0 and 1
	float direction;		// -1 or 1
	
	vector <ofPoint >  trail;
	
	ofPoint centroid;
	ofRectangle boundingRect;
	int nPts;
	ofPoint * pts;
	float energy;
	
	ofImage * spot;
	
	buildingShape();
	void draw();
	void update();
	void checkInside(float x, float y);
};