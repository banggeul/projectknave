#include "ofMain.h"

#include <fstream>
#include "ofxXmlSettings.h"

#include "ofxFileSharingServer.h"
#include "ofxFileSharingClient.h"
#include "ExamplePacket.h"

class testApp : public ofBaseApp{
public:
	testApp(){}
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	string mode;

	ofxXmlSettings settings;
	ofxFileSharingServer<ExamplePacket> dataServer;
	ofxFileSharingClient<ExamplePacket> dataClient;
};
