#pragma once

#include "ofMain.h"

class BinaryPacket : public ofxFilePacket {
public:
	vector<ofPoint> points;

	BinaryPacket() {
	}

	void save(ostream& out) const {
		binaryWrite(out, points);
		cout << "saving " << points.size() << " to file" << endl;
		for(int i = 0; i < points.size(); i++)
			cout << points[i].x << " " << points[i].y << endl;
	}

	void load(istream& in) {
		binaryRead(in, points);
		cout << "loaded " << points.size() << " points:" << endl;
		for(int i = 0; i < points.size(); i++) {
			cout << points[i].x << " " << points[i].y << endl;
		}
	}
};
