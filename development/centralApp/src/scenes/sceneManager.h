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

#include "HandScene.h"
#include "MonsterScene.h"
#include "StarScene.h"
#include "TreeScene.h"
#include "PaintScene.h"

#include "FerryBuilding.h"
#include "ofxBox2d.h"

#include "ofCvBlobTracker.h"		// <--- we all get a nice blob tacker too :)

#include "cvManager.h"

enum {
	HAND_SCENE, TREE_SCENE, PAINT_SCENE, MONSTER_SCENE, STAR_SCENE
};


class sceneManager : public ofCvBlobListener {

	public :

	void setup(cvManager& CVM);
	void update();
	void draw();
	void drawTop();

	void nextScene();
	void prevScene();
	void gotoScene(int theScene);

	void passInPacket(computerVisionPacket * packet);


	// keyboard and mouse
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int wx, int wy, int x, int y );
	void mouseDragged(int wx, int wy, int x, int y, int button);
	void mousePressed(int wx, int wy, int x, int y, int button);
	void mouseReleased(int wx, int wy, int x, int y, int button);

	// blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );
    void blobOff( int x, int y, int bid, int order );

	vector <ofxCvBlob>		networkBlobs;
	ofCvBlobTracker			tracker;

	int						numScenes;
	baseScene **			scenes;
	int						currentScene;

	cvManager* CVM;
};
