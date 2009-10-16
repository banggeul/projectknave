#include "ofxDaito.h"

ofxXmlSettings ofxDaito::xml;
vector<ofxOscSender> ofxDaito::senders;

void ofxDaito::send(ofxOscMessage& msg) {
	for(int i = 0; i < senders.size(); i++) {
		senders[i].sendMessage(msg);
	}
}

void ofxDaito::setup(string settings, bool verbose) {
	xml.setVerbose(verbose);
	if(xml.loadFile(settings)) {
		int receivers = xml.getNumTags("receiver");
		for(int i = 0; i < receivers; i++) {
			xml.pushTag("receiver", i);
			string host = xml.getValue("host", "localhost");
			int port = xml.getValue("port", 0);
			senders.push_back(ofxOscSender());
			senders.back().setup(host, port);
			cout << "Connected OSC to " << host << ":" << port << endl;
			xml.popTag();
		}
	} else {
		cout << "Couldn't load file " << settings << endl;
	}
}

void ofxDaito::scene(int scene) {
	ofxOscMessage msg;
	msg.setAddress("/scene");
	msg.addIntArg(scene);
	send(msg);
}

void ofxDaito::sendCustom(ofxOscMessage &msg){
	send(msg);
}

void ofxDaito::bang(int scene,
		string eventName,
		float energy,
		float pan) {
	ofxOscMessage msg;
	msg.setAddress("/bang");
	msg.addStringArg(eventName);
	msg.addIntArg(scene);
	msg.addFloatArg(energy);
	msg.addFloatArg(pan);
	send(msg);
}
