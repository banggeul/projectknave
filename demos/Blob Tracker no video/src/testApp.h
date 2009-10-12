#ifndef _TEST_APP
#define _TEST_APP


#include "ofxOpenCv.h"
#include "ofMain.h"


//the two things we need for our blob tracker
#include "blobTracker.h"
#include "trackedBlob.h"


#include "WiggleMonster.h"


class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased (int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	int cw, ch;
	
	ofVideoPlayer video;
	
	ofxCvGrayscaleImage cvThresh;
	ofxCvGrayscaleImage cvBackground;
	ofxCvGrayscaleImage cvGray;
	ofxCvColorImage cvColor;
	
	ofxCvContourFinder contourFinder;
	
	blobTracker tracker;
	
	//vector is like an array - but it is more flexible
	//think of a vector as a c++ array
	vector <trackedBlob> trackedBlobs;
	
	int threshold;
	int backSubMode;
	int distanceThresh;
	
	ofPoint biggestBlobPos;
	
	
	
	// Monsters
	
	vector <WiggleMonster> wiggleMonsters;
	
	
	
};

#endif

