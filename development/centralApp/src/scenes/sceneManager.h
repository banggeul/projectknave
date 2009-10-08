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

#include "BodyScene.h"
#include "MonsterScene.h"
#include "StarScene.h"
#include "TreeScene.h"
#include "PaintScene.h"

class sceneManager {
	
	public :
	
	void setup();
	void update();
	void draw();
	
	void nextScene();
	void prevScene();
	void gotoScene(int theScene);
	
	void passInPacket(computerVisionPacket * packet);
	
	
	// keyboard and mouse
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	
	
	int						numScenes;
	baseScene **			scenes;
	int						currentScene;
	
};