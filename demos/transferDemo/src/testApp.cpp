#include "testApp.h"

void testApp::setup() {
	ofBackground(255, 255, 255);
	settings.loadFile("settings.xml");

	string server = settings.getValue("server", "127.0.0.1");
	int port = settings.getValue("port", 9999);
	string filename = settings.getValue("filename", "shared.bin");
	mode = settings.getValue("mode", "client");

	if(mode == "server")
		dataServer.setup(filename, port);
	else if(mode == "client")
		dataClient.setup(filename, server, port);
}

void testApp::update() {
	if(mode == "client")
		dataClient.update();
}

void testApp::draw() {
}

void testApp::keyPressed(int key) {
	if(mode == "server") {
		ExamplePacket packet;
		packet.points.push_back(ofPoint(5, 10));
		packet.points.push_back(ofPoint(15, 20));
		packet.points.push_back(ofPoint(25, 30));
		dataServer.send(packet);
	}
}
