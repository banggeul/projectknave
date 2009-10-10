

#pragma once;

#include "ofMain.h"
#include "ofxXmlSettings.h"

// simpe shape holder for points
class FerryShape {
public:
	int				 num;
	vector <ofPoint> pnts;	
};



class FerryBuilding {
	
public:
	
	bool					bSaved;
	bool					bFirstPoint;
	bool					bEnable;
	ofImage					building;
	ofImage					mask;		
	vector <FerryShape>		shapes; 
	ofxXmlSettings			xmlSaver;
	
	
	// ------------------------------------
	void setupBuilding();
	void draw();
	void drawContour();
	void clear();
	
	void saveBuilding();
	
	
	void update() {}
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	
	
	
	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}
	
	
};