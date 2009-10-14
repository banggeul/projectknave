#pragma once

#include <fstream>
#include "ofxNetwork.h"
#include "ofxFilePacket.h"

template <class T>
class ofxFileSharingClient {
protected:
	string filename;
	ofxTCPClient tcpClient;
	T packet;
	long lastAttempt;
	static const int reconnectTime = 500;
public:
	ofxFileSharingClient() {
		lastAttempt = ofGetElapsedTimeMillis();
	}
	void setup(string filename, string ip, int port) {
		this->filename = filename;
		tcpClient.setup(ip, port);
	}
	bool update() {
		if(tcpClient.isConnected()) {
			if(tcpClient.receive() == "new") {
				ifstream file(filename.c_str(), ios::out | ios::binary);
				if(!file.is_open()) {
					cout << "ofxFileSharingClient couldn't open shared file '" << filename << "'" << endl;
				} else {
					T curPacket;
					file >> curPacket;
					packet = curPacket;
					file.close();
				}
				return true;
			}
		} else {
			long curTime = ofGetElapsedTimeMillis();
			if(curTime - lastAttempt > reconnectTime) {
				tcpClient.setup(tcpClient.getIP(), tcpClient.getPort());
				lastAttempt = curTime;
			}
		}
		return false;
	}
	T& getPacket() {
		return packet;
	}
};
