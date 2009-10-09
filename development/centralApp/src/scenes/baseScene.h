/*
 *  baseScene.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */




#pragma once;

#include "ofMain.h"
#include "cvPacket.h"
#include "FerryBuilding.h"
#include "renderConstants.h"

class baseScene{
	
public: 
	
	virtual void setup(){}
	virtual void update(){}
	virtual void draw(){}
	
	virtual void keyPressed(int key){}
	virtual void keyReleased(int key){}
	virtual void mouseMoved(int x, int y){}
	virtual void mouseDragged(int x, int y, int button){}
	virtual void mousePressed(int x, int y, int button){}
	virtual void mouseReleased(int x, int y, int button){}

	// this is kind of ghetto
	// but for now, every scene will have the packet data for doing 
	// stuff with.
	// later we might have an object sitting between the raw data and
	// the scenes, smoothing or post processing
	
	computerVisionPacket		packet;	
	FerryBuilding				building;
};