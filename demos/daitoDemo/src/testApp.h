#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxDaito.h"

#define HOST "localhost"
#define PORT 12345

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
};

#endif
