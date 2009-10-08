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

class PaintScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	
	int tempC;
	
};