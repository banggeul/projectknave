
#pragma once;

#include "ofMain.h"
#include "baseScene.h"

#include "TreeConstants.h"
#include "Stem.h"
#include "TreePerson.h"
#include "ofxSvg.h"


class TreeScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();

	// ---------------------- key and mouse
	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	
	void load();
	void save();
	
	// ---------------------- blob tracking
	void blobOn( int x, int y, int bid, int order );
    void blobMoved( int x, int y, int bid, int order );    
    void blobOff( int x, int y, int bid, int order );
	
	int						minSpawnToGrowW, minSpawnToGrowH;
	int						curStem;
	Stem					stems[MAX_STEMS];
	
	ofxXmlSettings			xmlSaver;

	// ---------------------- Tree People
	vector <TreePerson>		treePeople;
	vector <ofxSvg>			leaves;

};