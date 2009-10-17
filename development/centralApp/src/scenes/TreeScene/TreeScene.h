
#pragma once;

#include "ofMain.h"
#include "baseScene.h"

#include "TreeConstants.h"


#include "RandomFern.h"
#include "ofxControlPanel.h"
#include "TreeBlob.h"
#include "MagicTree.h"
#include "particle.h"
#include "ButterFlyParticle.h"
#include "ButterflyAnimation.h"


class TreeScene : public baseScene {
	
public: 
	
	vector <int> butterFlyColor;
	
	void setup();
	void update();
	void draw();
	void drawTop();
	void updateFlocking();
	
	// ---------------------- key and mouse
	void keyPressed(int key);
	void mouseDragged(int wx, int wy, int x, int y, int button);
	void mousePressed(int wx, int wy, int x, int y, int button);
	void mouseReleased(int wx, int wy, int x, int y, int button);
	
	
	void load();
	void save();
	
	// ---------------------- blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	
	// ---------------------- offset & scale
	float					W, H;
	float					TREE_SCALE;
	
	// ---------------------- key and mouse
	int						minSpawnToGrowW, minSpawnToGrowH;
	int						curStem;
	
	ofxXmlSettings			xmlSaver;
	ofImage theDot;
	
	// ---------------------- Tree People
	vector <TreeBlob>				treeBlobs;
	vector <ButterFlyParticle>		butterflys;
	vector <MagicTree>				trees;
	
	// ---------------------- Ferns
	vector <RandomFern>				ferns;
	int								fernTimer;
	
	
	
	// ---------------------- Pcket info
	bool					bGotMyFirstPacket;
	ofxControlPanel			panel;

	
};









