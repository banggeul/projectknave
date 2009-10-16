#pragma once

#include "ofxOsc.h"
#include "ofxXmlSettings.h"

/*
	At first, call ofxDaito::setup("setting.xml").
	At the beginning of each scene, call ofxDaito::start(sceneNumber)
	At the end of each scene, call ofxDaito::stop(sceneNumber)
	To trigger a sound event, call:
		ofxDaito::bang(sceneNumber, eventName, energy, pan)
		 energy is 0.0f - 1.0f, by default 1.0f
		 pan is 0.0f - 1.0f, by defualt 0.5f
*/

class ofxDaito {
private:
	static ofxXmlSettings xml;
	static string host;
	static int port;
	static vector<ofxOscSender> senders;
	static void send(ofxOscMessage& msg);
public:
	static void setup(string settings, bool verbose = true);
	static void scene(int scene);
	static void bang(int scene,	string eventName,	float energy = 1.0f, float pan = 0.5f);
	static void sendCustom(ofxOscMessage &msg);
};
