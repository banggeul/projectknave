#include "ofxDaito.h"

ofxXmlSettings ofxDaito::xml;
string ofxDaito::host;
int ofxDaito::port;
ofxOscSender ofxDaito::sender;

void ofxDaito::send(ofxOscMessage& msg) {
	sender.sendMessage(msg);
}

void ofxDaito::setup(string settings, bool verbose) {
	xml.setVerbose(verbose);
	if(xml.loadFile(settings)) {
		host = xml.getValue("host", "localhost");
		port = xml.getValue("port", 0);
		sender.setup(host, port);
	} else {
		cout << "Couldn't load file " << settings << endl;
	}
}

void ofxDaito::start(int scene) {
	ofxOscMessage msg;
	msg.setAddress("start");
	msg.addIntArg(scene);
	send(msg);
}

void ofxDaito::stop(int scene) {
	ofxOscMessage msg;
	msg.setAddress("stop");
	msg.addIntArg(scene);
	send(msg);
}

void ofxDaito::bang(int scene,
		string eventName,
		float energy,
		float pan) {
	ofxOscMessage msg;
	msg.setAddress("bang");
	msg.addStringArg(eventName);
	msg.addIntArg(scene);
	msg.addFloatArg(energy);
	msg.addFloatArg(pan);
	send(msg);
}
