
#pragma once;

#include "ofMain.h"
#include "baseScene.h"

#include "TreeConstants.h"
#include "Stem.h"
#include "TreePerson.h"
#include "ofxSvg.h"

#include "particle.h"
#include "ButterFly.h"

class TreeScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();
	void updateFlocking();
	
	// ---------------------- key and mouse
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	void load();
	void save();
	
	// ---------------------- blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	
	// ---------------------- offset & scale
	float					W, H;
	
	
	// ---------------------- key and mouse
	int						minSpawnToGrowW, minSpawnToGrowH;
	int						curStem;
	Stem					stems[MAX_STEMS];
	
	ofxXmlSettings			xmlSaver;
	ofImage theDot;
	// ---------------------- Tree People
	vector <TreePerson>		treePeople;
	vector <ButterFly>		butterflys;
	vector <MagicTree>		trees;
	
	// ---------------------- Pcket info
	bool					bGotMyFirstPacket;
	
	
};









