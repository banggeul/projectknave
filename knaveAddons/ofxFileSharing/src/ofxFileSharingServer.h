#pragma once

#include <fstream>
#include "ofxNetwork.h"
#include "ofxFilePacket.h"

template <class T>
class ofxFileSharingServer {
protected:
	string filename;
	ofxTCPServer tcpServer;
	bool verbose;
public:
	ofxFileSharingServer() {
		verbose = true;
	}
	void setup(string filename, int port) {
		if(verbose)
			cout << "ofxFileSharingServer: connecting to " << port << "/" << filename << endl;
		this->filename = filename;
		tcpServer.setup(port);
	}
	void send(T& packet) {
		ofstream file(filename.c_str(), ios::out | ios::binary);
		if(!file.is_open()) {
			if(verbose)
				cout << "ofxFileSharingServer: couldn't open shared file '" << filename << "'" << endl;
		} else {
			file << packet;
			file.close();
			tcpServer.sendToAll("new");
		}
	}
};
