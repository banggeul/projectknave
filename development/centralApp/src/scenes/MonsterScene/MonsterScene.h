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
#include "MonsterParticles.h"
#include "ofxBox2d.h"
#include "MonsterSVGParts.h"





#define MAX_MONSTER_PARTICLES 20



class MonsterScene : public baseScene {
	
public: 
	
	// ---------------------- 
	void setup();
	void update();
	void draw();

	
	// ---------------------- mouse & keyboard
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);

	// ---------------------- blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	// get a monster here
	BubbleMonster&  getMonsterById( int monsterId );
	
	
	// ---------------------- utils
	void createBuildingContour();
	void drawTop();
	
	
	// ---------------------- settings
	bool						bGotMyFirstPacket;
	
	// ---------------------- 
	// Ferry Building
	FerryBuilding				ferryBuilding;
	vector <ofxBox2dLine>		box2dBuilding;

	
	// tracking
	int							lastFrameRendered;
	
	// particles
	ofxBox2d					box2d;
	int							particleCount;
	vector <MonsterParticles>	monsterParticles;
	
	
	// monsters
	MonsterSVGParts				parts;
	ofxContourAnalysis			contourAnalysis;
	vector <BubbleMonster>		monsters;
	
};