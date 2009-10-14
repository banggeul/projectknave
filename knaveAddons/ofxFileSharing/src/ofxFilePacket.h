#pragma once

#include "ofMain.h"

class ofxFilePacket {
public:
	ofxFilePacket() {}
	virtual void save(ostream& out) const = 0;
	virtual void load(istream& in) = 0;
};

template <class T>
void binaryWrite(ostream& out, const T& t) {
	out.write(reinterpret_cast<const char*>(&t), sizeof(t));
}

template <class T>
void binaryWrite(ostream& out, const vector<T>& v) {
	binaryWrite<int>(out, v.size());
	for(int i = 0; i < v.size(); i++)
		binaryWrite<T>(out, v[i]);
}

template <class T>
void binaryRead(istream& in, T& t) {
	in.read(reinterpret_cast<char*>(&t), sizeof(t));
}

template <class T>
void binaryRead(istream& in, vector<T>& v) {
	int size;
	binaryRead<int>(in, size);
	for(int i = 0; i < size; i++) {
		v.push_back(T());
		binaryRead<T>(in, v.back());
	}
}

ostream& operator<<(ostream& out, const ofxFilePacket& packet);
istream& operator>>(istream& in, ofxFilePacket& packet);
