/*
 *  sceneManager.h
 *  openFrameworks
 *
 *  Created by Zach Lieberman on 9/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once;

#include "ofMain.h"
#include "simpleScene.h"
#include "baseScene.h"

class sceneManager {
	
	public :
	
	void setup();
	void update();
	void draw();
	
	void nextScene();
	void prevScene();
	
	void passInPacket(computerVisionPacket * packet);
	
	
	
	int						numScenes;
	baseScene **			scenes;
	int						currentScene;
	
};