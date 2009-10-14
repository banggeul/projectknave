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

// the funky monsters
#include "BubbleMonster.h"
#include "ofxContourAnalysis.h"


class MonsterScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();

	
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	BubbleMonster&  getMonsterById( int monsterId );
	
	
	// tracking
	int						lastFrameRendered;
	
	// monsters
	ofxContourAnalysis	    contourAnalysis;
	vector <BubbleMonster>	monsters;
	
};