#include "testApp.h"

void testApp::setup() {
	ofBackground(255, 255, 255);
	settings.loadFile("settings.xml");

	string server = settings.getValue("server", "127.0.0.1");
	int port = settings.getValue("port", 9999);
	string filename = settings.getValue("filename", "shared.bin");
	mode = settings.getValue("mode", "client");

	if(mode == "server" || mode == "both")
		dataServer.setup(filename, port);
	if(mode == "client" || mode == "both")
		dataClient.setup(filename, server, port);
}

void testApp::update() {
	if(mode == "client" || mode == "both")
		dataClient.update();
}

void testApp::draw() {
}

void testApp::keyPressed(int key) {
	if(mode == "server" || mode == "both") {
		ExamplePacket packet;
		for(int i = 0; i < 32; i++)
			packet.points.push_back(ofPoint(ofRandomuf(), ofRandomuf()));
		dataServer.send(packet);
	}
}
