

#pragma once;

#include "ofMain.h"

class FerryBuilding {
	
public:
	
	ofImage building;
	
	// ------------------------------------
	void setupBuilding(string buildingFile);
	void draw();
	
	void update(){}
	
	
	void keyPressed(int key){}
	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mousePressed(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	
	
};