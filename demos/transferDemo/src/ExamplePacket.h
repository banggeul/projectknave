#pragma once

#include "ofMain.h"

class ExamplePacket : public ofxFilePacket {
public:
	vector<ofPoint> points;

	ExamplePacket() {
	}

	void save(ostream& out) const {
		out << points.size() << ',';
		for(int i = 0; i < points.size(); i++)
			out << points[i].x << ',' << points[i].y << ',';

		cout << "just saved " << points.size() << " points" << endl;
		for(int i = 0; i < points.size(); i++)
			cout << points[i].x << " " << points[i].y << endl;
	}

	void load(istream& in) {
		int size;
		in >> size;
		in.ignore();
		for(int i = 0; i < size; i++) {
			float x, y;
			in >> x;
			in.ignore();
			in >> y;
			in.ignore();
			points.push_back(ofPoint(x, y));
		}

		cout << "just loaded " << points.size() << " points" << endl;
		for(int i = 0; i < points.size(); i++)
			cout << points[i].x << " " << points[i].y << endl;
	}
};
