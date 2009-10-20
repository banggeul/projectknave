#include "ofMain.h"

#include "ofxBox2d.h"
#include "ImageWarper.h"

class testApp : public ofBaseApp{
public:
	testApp(){}
	void setup();
	void update();
	void draw();

	ImageWarper warper;
};
