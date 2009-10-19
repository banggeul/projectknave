

#pragma once;

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxDirList.h"
#include "renderConstants.h"


// simple shape holder for points
class FerryShape {
public:
	int				 num;
	vector <ofPoint> pnts;
};




class FerryBuilding {

public:



	string					file;
	ofxXmlSettings			xmlSaver;
	ofxDirList				fileLister;

	vector <FerryShape>		shapes;

	bool					bSaved;
	bool					bFirstPoint;
	bool					bEnable;


	// file saving
	int						selectedFile;
	int						fileCount;
	vector <string>			files;

	// ------------------------------------
	void setupBuilding(string fileToLoad);
	void loadBuilding(string theFile);

	void draw();
	void drawContour();
	void drawInfo();

	void clear();
	void saveBuilding();



	// ------------------------------------
	void update() {}
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);



	void keyReleased(int key){}
	void mouseMoved(int x, int y){}
	void mouseDragged(int x, int y, int button){}
	void mouseReleased(int x, int y, int button){}


};
