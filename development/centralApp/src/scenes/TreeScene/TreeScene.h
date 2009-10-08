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

#include "Stem.h"

#define MAX_STEMS 10


class TreeScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	
	
	int		curStem;
	Stem	stems[MAX_STEMS];
	
	
	
};