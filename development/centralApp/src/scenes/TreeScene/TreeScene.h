
#pragma once;

#include "ofMain.h"
#include "baseScene.h"

#include "TreeConstants.h"
#include "Stem.h"


class TreeScene : public baseScene {
	
public: 
	
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	
	
	int		curStem;
	Stem	stems[MAX_STEMS];
	
	
	
};