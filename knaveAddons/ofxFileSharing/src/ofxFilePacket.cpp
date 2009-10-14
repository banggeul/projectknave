#include "ofxFilePacket.h"

ostream& operator<<(ostream& out, const ofxFilePacket& packet) {
	packet.save(out);
	return out;
}

istream& operator>>(istream& in, ofxFilePacket& packet) {
	packet.load(in);
	return in;
}
