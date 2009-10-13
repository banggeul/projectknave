#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "renderManager.h"

#include "ofxOpenCv.h"


class testApp : public ofSimpleApp{
	
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
	
	int				cw, ch;
	ofVideoGrabber  camera;
	
	renderManager		rm;
	ofImage				dualImage;
	ofRectangle			guiIn;
	ofRectangle			guiOut;

	// opencv
	ofxCvColorImage		color;
	ofxCvGrayscaleImage	gray;
	ofxCvContourFinder  contour;
	

};
#endif

