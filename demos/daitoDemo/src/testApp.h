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
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
	private:
		ofxOscSender sender;
};

#endif
