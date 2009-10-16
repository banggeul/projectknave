

#pragma once

#include "ofMain.h"
#include "baseScene.h"

// the funky monsters
#include "BubbleMonster.h"
#include "ofxContourAnalysis.h"
#include "MonsterParticles.h"
#include "ofxBox2d.h"
#include "MonsterSVGParts.h"
#include "ofxFBOTexture.h"
#include "MonsterBall.h"
#include "ofxDaito.h"
#include "MonsterConst.h"
#include "MonsterData.h"
#include "MonsterLine.h"
#include "ofxXmlSettings.h"
#include "MonsterWindow.h"
#include "MonsterDust.h"






/*
 OSC setup:
 
 EVENT: 
 newMonster
 monsterMove
 deadMonster
 
 DATA:
 scene			 = int
 type of monster = int
 id				 = int
 center x		 = float <-- normailized
 center y	     = float <-- normailized
 width			 = flaot <-- normailized
 height			 = float <-- normailized
 
 
 Particle Events:
 
 EVENT:
 particleHit
 
 DATA:
 scene			 = int
 x				 = float <-- normailized
 y				 = float <-- normailized
 
 
 */






class MonsterScene : public baseScene , public b2ContactListener{
	
public: 
	
	// ---------------------- 
	void setup();
	void update();
	void draw();
	void render();
	
	void saveMonsterSettings();
	void loadMonsterSettings();
	void cleanUpScene();
	
	// ---------------------- mouse & keyboard
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	// ---------------------- blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	// ---------------------- box2d bumps
	// a bit of a hack
	void Add(const b2ContactPoint* point);
	void Remove(const b2ContactPoint* point);
	
	
	ofPoint OFFSET;
	float W, H;
	
	
	// get a monster here
	BubbleMonster&				getMonsterById( int monsterId );
	
	
	// colors
	int							monsterColor[NUM_MONSTER_COLOR];
	
	
	// sounds for daito
	ofxDaito					DAITO;
	
	
	int							ballCounter;
	int							timeSinceLastBurst;
	
	// ---------------------- utils
	void createBuildingContour();
	void drawTop();
	
	ofxFBOTexture				FBO;
	ofImage						dotImage;
	
	// ---------------------- settings
	bool						bDebug;
	ofxXmlSettings				xmlSaver;
	vector <MonsterWindow>		windowPnts;
	bool						bGotMyFirstPacket;
	
	// ---------------------- 
	// Ferry Building
	FerryBuilding				ferryBuilding;
	vector <MonsterLine>		box2dBuilding;
	
	
	// tracking
	int							lastFrameRendered;
	int							lastTypeofMonster;
	
	
	// particles
	ofxBox2d					box2d;
	int							particleCount;
	vector <MonsterParticles>	monsterParticles;
	vector <MonsterDust>		dust;
	
	// monsters
	MonsterSVGParts				parts;
	ofxContourAnalysis			contourAnalysis;
	vector <BubbleMonster>		monsters;
	vector <MonsterBall>		balls;
	
	vector <ofPoint> sclContour;
};





