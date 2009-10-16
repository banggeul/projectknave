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

class StarScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	
	int tempC;
	
	void announceChange(int change);
	void keyPressed(int key);
	
	 particleManager PM;
		ofImage		temp;
	
};


/*

 
 
 temp.loadImage("images/logoHalf.png");
 temp.setImageType(OF_IMAGE_GRAYSCALE);
 PM.findLockTarget(temp.getPixels(), temp.width, temp.height);
 

 */
 